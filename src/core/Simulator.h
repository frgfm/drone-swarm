// Simulator.h
#pragma once

#include "RectPrism.h"
#include "CirclePrism.h"
#include <utility>
#include <vector>
#include <list>
#include <typeinfo>

namespace particle {
  class Simulator
  {
  public:
  double duration, simu_dt;      // Duration & step of simulation
  std::list<Rectangle> myRect; // List of Rectangle
  std::list<Circle> myParticles; // List of particules
  std::vector<Elements*> myElements; // Vector of Elements' pointers
  std::vector<std::pair<int,Point>> myLock; // Vector of target locked by elements
  std::vector<bool> myExplo;
  double vmax;
  Point target;
  Simulator();
  Simulator(const Point& min, const Point& max, double width, double duration, double simu_dt); // Empty simulation box
  
  void addRect(const Point& dim, double weight, const Point& pos, double angle, const Point& speed);
  void addRect(const Point& min, const Point& max);
  void makeBox(const Point& min, const Point& max, double width); // 4 Rectangles as edges
  void addUav(double r, double weight, const Point& pos, const Point& speed);
  void addUavs(const Point& min, const Point& max, int n, double r, double weight, double vmax); // add n random UAV in a specific area
  //private:
  void step_lin(double dt);  // Linearly update position without collision
  bool checkTraj(const Point& pos, double d1, const Point& target, double d2, double precision);
  void align(int k, const Point& target);
  bool contains(const std::vector<int> vect, int val);
  void push_dir(double d); // Change angle towards target
  Point getObst(const Point& pos, double radius, double ang, double precision);
  void explore(int i, double d);
  void push_target(const Point& t);

};
}

