#pragma once
#include "Point.h"
#include <utility>

namespace particle {
	class Prism
	{
	public:
		Prism()=default;
		
		virtual bool belongs(const Point& p, double t) const = 0; // does the point belong to the prism ? 
		virtual Point toss(double t) const = 0; // random Point in Prism
		
		// Monte Carlo method to get the duration before the collision between 2 prisms
		static std::pair<double,Point> MC(const Prism* p1, const Prism* p2, int nb_sample, double dt_step, double dt_max) ;
	};
}

