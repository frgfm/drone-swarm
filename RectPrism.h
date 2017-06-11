#pragma once

#include "Prism.h"
#include "Rectangle.h"

namespace particle {
	class RectPrism:public Prism
	{
	public:
		Rectangle r; // keeps the rectangle in argument
		RectPrism();
		RectPrism(const Rectangle& r);
		RectPrism(const Rectangle* r);

		bool belongs(const Point& p, double t) const;
		Point toss(double t) const;
	};
}

