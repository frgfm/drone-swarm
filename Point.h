// Point.h
#pragma once

namespace particle {
  class Point
  {
  public:
    double x,y;
    Point();
    Point(const Point& a);
    Point(double x, double y);
  // Operators
    bool operator==(const Point& p) const;
    Point& operator=(const Point& p);
    Point& operator+=(const Point& p);
    Point& operator-=(const Point& p);
    Point& operator*=(double a);
    Point& operator/=(double a);
  // Methods
    double norm2() const; // squared norm of the vector (0,0)->Point
    double norm() const;
    static Point randomRect(const Point& min, const Point& max); // Shoots a random point in Rectangle
    static Point shootIn(const Point& dim, const Point& pos, double a); // Shoots in rotated rectangle
    static Point randomCircle(const Point& centre, double r2); // Shoots a random point in Circle
  };
  // Operators
  Point operator+(const Point& p1, const Point& p2);
  Point operator-(const Point& p1, const Point& p2);
  Point operator*(const Point& p, double a);
  Point operator*(double a, const Point& p);
  Point operator/(const Point& p, double a);

}

