#include "RectPrism.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

particle::RectPrism::RectPrism(const particle::Rectangle& r) : Prism(), r(r) {} 
particle::RectPrism::RectPrism(const particle::Rectangle* r) : Prism(), r(r) {}

bool particle::RectPrism::belongs(const particle::Point& p, double t) const {
	particle::Point sp = r.cardSpeed();
	particle::Point centre = r.pos + sp*t;
	particle::Point z = p - centre;
	double a = r.angle + r.speed.y*t;
	double b = atan(z.y/z.x);
	particle::Point pol = z.norm()*particle::Point(cos(b-a),sin(b-a)); // polar coord
	if (pol.y <= r.dim.y/2 && pol.y >= -r.dim.y/2 && pol.x <= r.dim.x/2 && pol.x >= -r.dim.x/2) {return true;}
  else {return false;}
}

particle::Point particle::RectPrism::toss(double t) const {
	particle::Point sp = r.cardSpeed();
	particle::Point centre = r.pos + sp*t;
	double ang = r.angle + r.speed.y*t;
	return particle::Point::shootIn(r.dim,centre,ang);
}
