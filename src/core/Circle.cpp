#include "Circle.h"
#include "CirclePrism.h"
#include <math.h>
#include <iostream>

particle::Circle::Circle() : Elements(),r2(0)
{
}

particle::Circle::Circle(const particle::Circle* c) :  Elements(c->weight, c->pos, c->angle, c->speed, c->acc), r2(c->r2)
{
}

particle::Circle::Circle(const particle::Circle& c) :  Elements(c.weight, c.pos, c.angle, c.speed, c.acc), r2(c.r2)
{
}

particle::Circle::Circle(double r, double weight, const particle::Point& pos, double angle) : Elements(weight,pos,angle), r2(r*r)
{
}

particle::Circle::Circle(double r, double weight, const particle::Point& pos, double angle, const particle::Point& speed) : Elements(weight,pos,angle,speed), r2(r*r)
{
}

particle::Circle::Circle(double r, double weight, const particle::Point& pos, double angle, const particle::Point& speed, const particle::Point& acc) : Elements(weight,pos,angle,speed,acc), r2(r*r)
{
}


particle::Circle& particle::Circle::operator=(const particle::Circle& a) {
  if (this != &a) {
    this -> particle::Elements::operator=(a);
    r2 = a.r2;
  }
  return *this;
}

particle::Point particle::Circle::cardSpeed() const {
  double spx,spy;
  spx = speed.x*cos(angle);
  spy = speed.x*sin(angle);
  particle::Point sp(spx,spy);
  return sp;
}

particle::Prism* particle::Circle::make_prism() {
  particle::CirclePrism* cp = new particle::CirclePrism(this);
  return cp;
}

double particle::Circle::getR2() {
  return r2;
}

particle::Point particle::Circle::getDim() { 
  double leng = 2*sqrt(r2);
  particle::Point dim(leng,leng);
  return dim;
}

std::pair<particle::Point,particle::Point> particle::Circle::normalAt(const particle::Point& contact) {
  std::pair<particle::Point,particle::Point> n;
  n.first = pos;
  n.second = contact;
  return n;  // normalAt def by 2 points including the contact point
}

bool particle::Circle::is_free(){
  return true;
}
