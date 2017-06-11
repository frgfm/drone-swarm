#include "Elements.h"
#include <math.h>


particle::Elements::Elements(double weight, const particle::Point& pos, double angle)
: Elements(weight,pos,angle,Point(),Point()) {
}

particle::Elements::Elements(double weight, const particle::Point& pos, double angle, const particle::Point& speed)
: Elements(weight,pos,angle,speed,Point()) {
}

particle::Elements::Elements(double weight, const particle::Point& pos, double angle, const particle::Point& speed, const particle::Point& acc)
: weight(weight), pos(Point(pos.x, pos.y)), angle(angle), speed(Point(speed.x, speed.y)), acc(Point(acc.x, acc.y)) {
}

particle::Elements::Elements(double weight, const particle::Elements& e)
: Elements(weight,e.pos,e.angle,e.speed,e.acc) {
}

particle::Elements& particle::Elements::operator=(const particle::Elements& e) {
  if (this != &e) {
    weight = e.weight;
    pos = e.pos;
    angle = e.angle;
    speed = e.speed;
    acc = e.acc;
  }
  return *this;	
}

particle::Point particle::Elements::cardSpeed() const {
	double spx,spy;
	spx = speed.x*cos(angle);
	spy = speed.x*sin(angle);
	particle::Point sp(spx,spy);
	return sp;
}

// all remaining methods are pure virtual