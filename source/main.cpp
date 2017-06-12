#include <iostream>
#include "Simulator.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

int main()
{
	const double PI = 3.141592653589793;
	std::srand(std::time(0));
	particle::Point p0(0,0);
	particle::Point p1(0,0),p2(100,100),s1(1,0),s2(1,0),centre,p;
	particle::Point p3(10,10),p4(50,0), p5(55,60);
	particle::Point p6(25,25),p7(25,75), p8(75,25), p9(75,75), p10(50,50);
	
	// Test simulator + collide (OK) + rebound (not OK)
	
	particle::Simulator s(p1,p2,1,50,0.1);
	s.particle::Simulator::addRect(p4,p5);
	s.particle::Simulator::addUav(1,1,p6,0,s1);
	s.particle::Simulator::addUav(1,1,p7,0,s1);
	s.particle::Simulator::addUav(1,1,p8,PI,s1);
	s.particle::Simulator::addUav(1,1,p9,PI,s1);

	s.particle::Simulator::push_dir(p9,1);
	for (int i = 5; i < s.myElements.size(); i++){
		std::cout << "Angle of [" << i << "] : " << s.myElements[i]->angle/PI << " * Pi rad" << std::endl;
	}

	while(s.particle::Simulator::maxDist(p9) > 2){
		s.particle::Simulator::push_dir(p9,1);
		s.particle::Simulator::step_lin(1);
		std::cout << "Max dist : " << s.particle::Simulator::maxDist(p9) << std::endl;
	}
	
	

	
	//s.particle::Simulator::step(50);	

    srand(time(NULL));
	
	return 0;
}
