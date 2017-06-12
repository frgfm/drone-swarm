#include "Rectangle.h"
#include "RectPrism.h"
#include <algorithm>
#include <iostream>
#include <cmath>

particle::Rectangle::Rectangle() 
: particle::Elements(), dim(Point()) {}

particle::Rectangle::Rectangle(const particle::Rectangle* rect) 
: particle::Elements(rect->weight, rect->pos, rect->angle, rect->speed, rect->acc), dim(rect->dim) {}

particle::Rectangle::Rectangle(const particle::Rectangle& rect) 
: particle::Elements(rect.weight, rect.pos, rect.angle, rect.speed, rect.acc), dim(rect.dim) {}

particle::Rectangle::Rectangle(const particle::Point& min, const particle::Point& max) 
: particle::Elements(0,(min+max)/2,0), dim(Point(std::abs(max.x-min.x),std::abs(max.y-min.y))) {}

particle::Rectangle::Rectangle(const particle::Point& dim, double weight, const particle::Point& pos, double angle) 
: particle::Elements(weight,pos,angle), dim(dim) {}

particle::Rectangle::Rectangle(const particle::Point& dim, double weight, const particle::Point& pos, double angle, const particle::Point& speed) 
: particle::Elements(weight,pos,angle,speed), dim(dim) {}

particle::Rectangle::Rectangle(const particle::Point& dim, double weight, const particle::Point& pos, double angle, const particle::Point& speed, const particle::Point& acc) 
: particle::Elements(weight,pos,angle,speed,acc), dim(dim) {}

particle::Rectangle& particle::Rectangle::operator=(const particle::Rectangle& cp) {
  if(this != &cp) {
    this->particle::Elements::operator=(cp); // le = de la classe mère.
    dim = cp.dim;
  }
  return *this;
}

particle::Point particle::Rectangle::cardSpeed() const {
  double spx,spy;
  spx = speed.x*cos(angle);
  spy = speed.x*sin(angle);
  particle::Point sp(spx,spy);
  return sp;
}

particle::Prism* particle::Rectangle::make_prism() {
  particle::RectPrism* rp = new particle::RectPrism(this);
  return rp;
}

double particle::Rectangle::getR2() {return 0;}

particle::Point particle::Rectangle::getDim() {
  return dim;
}

std::pair<particle::Point,particle::Point> particle::Rectangle::normalAt(const particle::Point& contact) {
  std::pair<particle::Point,particle::Point> n;
  n.second = contact;
  // n.first = ?? incomplete
  return n;
}

bool particle::Rectangle::is_free(){
  return false;
}



