#include "Point.h"
#include <math.h>
#include <cstdlib>

particle::Point::Point() : x(0), y(0) {}

particle::Point::Point(const particle::Point& p) : x(p.x), y(p.y) {}

particle::Point::Point(double x, double y) : x(x), y(y) {}


bool  particle::Point::operator==(const particle::Point& p) const {
  return x == p.x && y == p.y;
}

particle::Point& particle::Point::operator=(const  particle::Point& p) {
  if (this != &p){
    x = p.x;
    y = p.y;
  }
  return *this;
}
particle::Point& particle::Point::operator+=(const particle::Point& p){
  x += p.x;
  y += p.y;
  return *this;
}

particle::Point& particle::Point::operator-=(const particle::Point& p){
  x -= p.x;
  y -= p.y;
  return *this;
}

particle::Point& particle::Point::operator*=(double a){
  x = a*x;
  y = a*y;
  return *this;
}

particle::Point& particle::Point::operator/=(double a){
  x = x/a;
  y = y/a;
  return *this;
}

particle::Point particle::operator+(const particle::Point& p1, const particle::Point& p2){
  particle::Point copy(p1);
  copy += p2;
  return copy;
}

particle::Point particle::operator-(const particle::Point& p1, const particle::Point& p2){
  particle::Point copy(p1);
  copy -= p2;
  return copy;
}

particle::Point particle::operator*(const particle::Point& p, double a){
  particle::Point copy(p);
  copy *= a;
  return copy;
}

particle::Point particle::operator*(double a, const particle::Point& p){
  particle::Point copy(p);
  copy *= a;
  return copy;
}

particle::Point particle::operator/(const particle::Point& p, double a){
  particle::Point copy(p);
  copy /= a;
  return copy;
}


double particle::Point::norm2() const {
  return x*x + y*y;
}

double particle::Point::norm() const {
  return sqrt(norm2());
}

particle::Point particle::Point::randomRect(const Point& min, const Point& max) {
  return particle::Point(min.x + (max.x-min.x)*(rand()/(1.0 + RAND_MAX)),min.y + (max.y-min.y)*(rand()/(1.0 + RAND_MAX)));
}

particle::Point particle::Point::shootIn(const Point& dim, const Point& pos, double a) {
  particle::Point v1,v2;
  v1 = (2*rand()/(1.0 + RAND_MAX))*dim.y/2*Point(cos(a),sin(a));
  v2 = (2*rand()/(1.0 + RAND_MAX))*dim.x/2*Point(-sin(a),cos(a));
  particle::Point res(pos+v1+v2);
  return res;
}

particle::Point particle::Point::randomCircle(const Point& centre, double r2) {
  particle::Point p(1,1), pmin, pmax;
  pmin = centre - p*sqrt(r2);
  pmax = centre + p*sqrt(r2);
  do { // for uniform distribution : random shot in circle and we keep only those in the circle
    p = particle::Point::randomRect(pmin, pmax);}
    while ((p-centre).norm2() > r2);
    return p;
  }

