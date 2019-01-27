#include "Prism.h"
#include <math.h>
#include <limits>
#include <utility>
#include <iostream>

std::pair<double,particle::Point> particle::Prism::MC(const particle::Prism* p1, const particle::Prism* p2, int nb_sample, double dt_step, double dt_max) {
  bool intersect = false;
  std::pair<double,particle::Point> mc;
  for (double t=0 ; t<dt_max && !intersect ; t+=dt_step){ // time loop
    for (int i=0 ; i < nb_sample && !intersect; i++){   // shooting loop
      particle::Point p = p1->toss(t);
      intersect = p2->belongs(p,t);
      if (intersect) {
       mc.first = t; // save the duration before the collision
       mc.second = p; // save pointers to the elements collidings
       return mc;}
     }
   }
   mc.first = std::numeric_limits<double>::infinity();
   return mc; // if no intersection, we send back infinite duration 
 }

