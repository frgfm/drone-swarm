// Elements.h
#pragma once
#include "Point.h"
#include <utility>

namespace particle {
  class Prism;  // forward declaration
  class Elements
  {
  public:
    double weight;
    Point pos;
    double angle;
    Point speed; // (speed norm, angular speed)
    Point acc; // (acc, angular acc)

    Elements()=default;
    Elements(double weight, const Point& pos, double angle);  // speed = 0, acc = 0
    Elements(double weight, const Point& pos, double angle, const Point& speed); // acc = 0
    Elements(double weight, const Point& pos, double angle, const Point& speed, const Point& acc); // complete
    Elements(double weight, const Elements& e);
    Elements& operator=(const Elements& p);

    Point cardSpeed() const;
    virtual Prism* make_prism() = 0;
    virtual double getR2() = 0; // Get the squared radius if Circle
    virtual Point getDim() = 0; // Get dimensions if Rectangle
    virtual std::pair<Point,Point> normalAt(const Point& contact) = 0;
    virtual bool is_free() = 0; // Get the mobility of the element for rebound
  };
}

