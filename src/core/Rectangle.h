// Rectangle.h
#pragma once
#include "Elements.h"

namespace particle {
    class Rectangle : public Elements {
    public:
        Point dim; // length, width
        Rectangle();
        Rectangle(const Point& min, const Point& max);
        Rectangle(const Point& dim, double weight, const Point& pos, double angle);
        Rectangle(const Point& dim, double weight, const Point& pos, double angle, const Point& speed);
        Rectangle(const Point& dim, double weight, const Point& pos, double angle, const Point& speed, const Point& acc);
        Rectangle(const Rectangle& rect);
        Rectangle(const Rectangle* rect);
        Rectangle& operator=(const Rectangle& rect);
        Point cardSpeed() const;
        virtual Prism* make_prism();
        virtual double getR2();
        virtual Point getDim();
        virtual std::pair<Point,Point> normalAt(const Point& contact);
        virtual bool is_free();
    };

}

