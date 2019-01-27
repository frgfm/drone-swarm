#include "CirclePrism.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>

particle::CirclePrism::CirclePrism(const particle::Circle& c) : Prism(), c(c) {}
particle::CirclePrism::CirclePrism(const particle::Circle* c) : Prism(), c(c) {}

bool particle::CirclePrism::belongs(const particle::Point& p, double t) const {
	particle::Point sp = c.cardSpeed();
	particle::Point centre = c.pos + sp*t;  // Projection of Prism in a time plane
	if ( (p - centre).norm2() < c.r2) {  // Is the point in that circle ?
		return true;}
		else {return false;}
}

particle::Point particle::CirclePrism::toss(double t) const {
	particle::Point sp = c.cardSpeed();
	particle::Point centre = c.pos + sp*t;  // Projection of Prism in a time plane
	return particle::Point::randomCircle(centre,c.r2); // random point in that circle
}
