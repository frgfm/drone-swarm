/*

g++ -o dessin dessin.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`
*/


#include "gtk/gtk.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include "Simulator.h"


/* ms */
#define REFRESH_PERIOD 50 // milliseconds


extern "C" {

  gboolean on_draw (GtkWidget *widget, cairo_t *cr, gpointer data) {

    particle::Simulator * simu_ptr = (particle::Simulator *)(g_object_get_data(G_OBJECT(widget),"simulation"));
    particle::Simulator& simu = *simu_ptr;


    // Rect
    for (int i=0 ; i < simu.myRect.size() ; i++) {
      cairo_set_source_rgb(cr,0,0,0); //black
      cairo_rectangle(cr, (simu.myElements[i]->pos.x-simu.myElements[i]->getDim().x/2), (simu.myElements[i]->pos.y-simu.myElements[i]->getDim().y/2), simu.myElements[i]->getDim().x, simu.myElements[i]->getDim().y);
      cairo_stroke (cr);

    }

    // Circle
    for (int i=simu.myRect.size() ; i < simu.myElements.size() ; i++) {
      //Shape
      cairo_set_source_rgb (cr, 0.8, 0.8, 0); // yellow
      cairo_arc (cr, simu.myElements[i]->pos.x, simu.myElements[i]->pos.y, sqrt(simu.myElements[i]->getR2()), 0, 2 * M_PI);
      cairo_stroke (cr); // draw
      // Direction of Circle
      cairo_set_source_rgb(cr, 1.0, 0, 0);
      cairo_set_line_width (cr,2);
      cairo_move_to(cr, simu.myElements[i]->pos.x, simu.myElements[i]->pos.y);
      double px,py;
      px = simu.myElements[i]->pos.x + sqrt(simu.myElements[i]->getR2())*cos(simu.myElements[i]->angle);
      py = simu.myElements[i]->pos.y + sqrt(simu.myElements[i]->getR2())*sin(simu.myElements[i]->angle);
      cairo_line_to(cr, px,py);
      cairo_stroke (cr);
    }

    return FALSE;
  }

  // Bouton Reset
  void on_reset (GtkButton *button, gpointer user_data) {
    particle::Simulator * simu_ptr = (particle::Simulator *)(g_object_get_data(G_OBJECT(button),"simulation"));
    particle::Simulator& simu = *simu_ptr;
    simu.push_target(particle::Point(0,0));
    std::cout << "RESET" << std::endl;

  }

  // Bouton Toggle
  void on_toggle (GtkButton *button, gpointer user_data) {
    particle::Simulator * simu_ptr = (particle::Simulator *)(g_object_get_data(G_OBJECT(button),"simulation"));
    particle::Simulator& simu = *simu_ptr;

    particle::Point po(50,100), sp(10,0);
    simu.myElements[4]->pos = po;
    simu.myElements[4]->speed = sp;
    // Forcer le redessin...
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(user_data), "dessin")));
  }
}

gboolean on_time(gpointer user_data) { // function called by g_timeout_add every REFRESH_PERIOD
  GtkWidget* dessin = (GtkWidget*)user_data;
  particle::Simulator * simu_ptr = (particle::Simulator *)(g_object_get_data(G_OBJECT(dessin),"simulation"));
  particle::Simulator& simu = *simu_ptr;
  simu.push_dir(0);
  simu.step_lin(simu.simu_dt);
  gtk_widget_queue_draw(dessin);


  return TRUE;
}

gboolean btn_press_callback(GtkWidget *btn, GdkEventButton *event, gpointer user_data){
  GtkWidget* dessin = (GtkWidget*)user_data;
  particle::Simulator * simu_ptr = (particle::Simulator *)(g_object_get_data(G_OBJECT(dessin),"simulation"));
  particle::Simulator& simu = *simu_ptr;
  particle::Point click(event->x,event->y), sp(0,0);
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
    {//right mouse click
      simu.particle::Simulator::addUav(5,1,click,sp);
      return true;//or false
  }
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1)
    {//left mouse click
      std::cout << "New target set at (" << event->x << "," << event->y << ")" << std::endl;
      simu.push_target(click);
  }
}


int main(int argc, char* argv[]) {
    // Create environment
   std::srand(std::time(0));
   GtkBuilder  *  p_builder   = NULL;
   GError      *  p_err       = NULL;

  // init simulator
  particle::Point p0(0,0);
  particle::Point p1(0,0),p2(450,250);
  particle::Simulator simu(p1,p2,5,250,0.1);
  simu.push_target(p0);
  simu.vmax = 10;

  // create environment
  particle::Point p3(50,0), p4(55,60);
  simu.particle::Simulator::addRect(p3,p4);
  particle::Point p5(0,150), p6(200,155);
  simu.particle::Simulator::addRect(p5,p6);
  particle::Point p7(200,50),p8(205,155);
  simu.particle::Simulator::addRect(p7,p8);
  particle::Point min(100,150),checkPt(100,155);
  std::cout << simu.particle::Simulator::checkTraj(min, 1, checkPt, 0.1, 0.1) << std::endl;

   // Initialisation of GTK+
   gtk_init (& argc, & argv);
   // New GtkBuilder
   p_builder = gtk_builder_new ();

   if (p_builder != NULL) // loading of xml/glade in p_builder
      gtk_builder_add_from_file (p_builder, "View.glade", & p_err);
   else
     return EXIT_FAILURE;

   if (p_err != NULL) {
     std::cerr << p_err->message << std::endl;
     g_error_free (p_err);
     return EXIT_FAILURE;
   }
   
   GtkWidget * window = GTK_WIDGET(gtk_builder_get_object (p_builder, "mainWindow"));
   gtk_builder_connect_signals(p_builder,
			       (gpointer)p_builder); // the data field of each callback will be the builder.

   // Hook up ....
   g_object_set_data(gtk_builder_get_object(p_builder, "dessin"),"simulation",(gpointer)(&simu));
   g_object_set_data(gtk_builder_get_object(p_builder, "reset"),"simulation",(gpointer)(&simu));
   g_object_set_data(gtk_builder_get_object(p_builder, "toggle"),"simulation",(gpointer)(&simu));

   // Start of timeout
   guint timout = g_timeout_add(REFRESH_PERIOD,
				on_time,
				(gpointer)(gtk_builder_get_object(p_builder, "dessin")));

   g_signal_connect(G_OBJECT(window), "button-press-event",G_CALLBACK(btn_press_callback),(gpointer)(gtk_builder_get_object(p_builder, "dessin")));


   gtk_widget_show_all (window);
   gtk_main ();
   g_object_unref(G_OBJECT(p_builder));
   return EXIT_SUCCESS;

}

