// Circle.h
#pragma once
#include "Elements.h"

namespace particle {
	class Circle : public Elements {
	public:
		double r2;
		Circle();
		Circle(double r, double weight, const Point& pos, double angle);
		Circle(double r, double weight, const Point& pos, double angle, const Point& speed);
		Circle(double r, double weight, const Point& pos, double angle, const Point& speed, const Point& acc);
		Circle(const Circle& c);
		Circle(const Circle* c);
		Circle& operator=(const Circle& c);
		Point cardSpeed() const;
		virtual Prism* make_prism(); // Creates a prism on demand
		virtual double getR2();
		virtual Point getDim();
		virtual std::pair<Point,Point> normalAt(const Point& contact);
		virtual bool is_free();
	};
}

