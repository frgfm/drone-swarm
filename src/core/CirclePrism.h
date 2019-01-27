#pragma once

#include "Circle.h"
#include "Prism.h"

namespace particle {
	class CirclePrism:public Prism
	{
	public:
		Circle c;
		CirclePrism();
		CirclePrism(const Circle& c);
		CirclePrism(const Circle* c);

		bool belongs(const Point& p, double t) const; // Point belongs to prism ?
		Point toss(double t) const;  // creates a random point in the Prism at t
	};
}

