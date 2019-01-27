#include "Simulator.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <algorithm>

particle::Simulator::Simulator(const Point& min, const Point& max, double width, double duration, double simu_dt) : duration(duration), simu_dt(simu_dt), target(Point()) {
  // Box creation
  particle::Simulator::makeBox(min,max,width);

}

void particle::Simulator::addRect(const particle::Point& dim, double weight, const particle::Point& pos, double angle, const particle::Point& speed){
  myRect.push_back(particle::Rectangle(dim,weight,pos,angle,speed));  // List of Rectangles
  particle::Elements* last = &(myRect.back());
  myElements.push_back(last);  // Pointer  --> particle Rect
  std::pair<int,Point> par;
  par.first = 0;
  par.second = Point();
  myLock.push_back(par); // Non locked target
  myExplo.push_back(false);
  std::cout << "Element[" << myElements.size()-1<< "] (Rect) at (" << myElements[myElements.size()-1]->pos.x << "," << myElements[myElements.size()-1]->pos.y << ") added" << std::endl;
}

void particle::Simulator::addRect(const particle::Point& min, const particle::Point& max){
  myRect.push_back(particle::Rectangle(min,max));  // List of Rectangles
  particle::Elements* last = &(myRect.back());
  myElements.push_back(last);  // Pointer  --> particle Rect
  std::pair<int,Point> par;
  par.first = 0;
  par.second = Point();
  myLock.push_back(par); // Non locked target
  myExplo.push_back(false);
  std::cout << "Element[" << myElements.size()-1<< "] (Rect) at (" << myElements[myElements.size()-1]->pos.x << "," << myElements[myElements.size()-1]->pos.y << ") added" << std::endl;
}

void particle::Simulator::makeBox(const particle::Point& min, const particle::Point& max, double width) {
  
  particle::Simulator::addRect(particle::Point(min.x,min.y-width),particle::Point(max.x,min.y));
  particle::Simulator::addRect(particle::Point(min.x-width,min.y),particle::Point(min.x,max.y));
  particle::Simulator::addRect(particle::Point(min.x,max.y),particle::Point(max.x,max.y+width));
  particle::Simulator::addRect(particle::Point(max.x,min.y),particle::Point(max.x+width,max.y));
}

void particle::Simulator::addUav(double r, double weight, const particle::Point& pos, const particle::Point& speed){
  const double PI = 3.141592653589793;
  double angle = rand()/(1.0 + RAND_MAX) * 2.0 * PI;
  myParticles.push_back(particle::Circle(r,weight,pos,angle,speed));  // List of particles
  particle::Elements* last = &(myParticles.back());
  myElements.push_back(last);  // Pointer  --> particle Circle
  std::pair<int,Point> par;
  par.first = 0;
  par.second = Point();
  myLock.push_back(par); // No locked target
  myExplo.push_back(false);
  std::cout << "Element[" << myElements.size()-1<< "] (UAV) at (" << myElements[myElements.size()-1]->pos.x << "," << myElements[myElements.size()-1]->pos.y << ") added" << std::endl;
}

void particle::Simulator::addUavs(const particle::Point& min, const particle::Point& max, int n, double r, double weight, double vmax){
  const double PI = 3.141592653589793;
  for(int i=0 ; i<n ; i++){
    particle::Point pos(particle::Point::randomRect(min,max)); // Point in box
    double a = rand()/(1.0 + RAND_MAX) * 2.0 * PI;
    double spx = (2*rand()/(1.0 + RAND_MAX)-1)*vmax;
    double spy = (2*rand()/(1.0 + RAND_MAX)-1)*vmax;
    particle::Point sp(spx,spy);
    particle::Simulator::addUav(r,weight,pos,sp);
  }
}

void particle::Simulator::step_lin(double dt) {
  for (double t=0 ; t<dt ; t+= simu_dt){
    for(auto i1 = myParticles.begin(); i1 != myParticles.end(); ++i1) {
      auto part_ptr = *i1;
      particle::Point sp = i1->cardSpeed();
      i1->pos = i1->pos + sp*simu_dt; // updates position knowing speed
    }
  }
}

bool particle::Simulator::checkTraj(const particle::Point& pos, double d1, const particle::Point& target, double d2, double precision){
  particle::Point dir(precision*(target-pos)/(target-pos).norm());
  int start(1+(int)(d1/precision));
  int endc((int)(((target-pos).norm()-d2)/precision));
  for (int i = start ; i < endc ; i++){
    particle::Point shot(pos + i*dir);
    // Check if shot intercepts other elements
    for (int j=0 ; j < myElements.size() ; j++){
      particle::Prism* pr = myElements[j]->make_prism();
      if (pr->belongs(shot,0)){
        //std::cout << "Shot n°" << i << " at (" << shot.x << "," << shot.y << ")" << std::endl;
        return false;
      }
    }
  }
  return true;
}

void particle::Simulator::align(int k, const particle::Point& target){
  const double PI = 3.141592653589793;
  myExplo[k] = false;
  // Vertical alignement
  if (target.x == myElements[k]->pos.x) {
    if (target.y > myElements[k]->pos.y) {myElements[k]->angle = PI/2;}
    else if (target.y < myElements[k]->pos.y){myElements[k]->angle = -PI/2;}
    else {myElements[k]->speed.x = 0;}
  }
  // Horizontal alignment
  else if (target.y == myElements[k]->pos.y){
    if(target.x > myElements[k]->pos.x){myElements[k]->angle = 0;}
    else {myElements[k]->angle = PI;}
  }
  // Remaining cases
  else {
    double ang = atan((target.y-myElements[k]->pos.y)/(target.x-myElements[k]->pos.x));
    if (target.x < myElements[k]->pos.x){
      if(target.y<myElements[k]->pos.y){
        ang = ang - PI;
      }
      else{ang = ang + PI;}
    }
    myElements[k]->angle = ang;
    myElements[k]->speed.y = 0;
  }
}

bool particle::Simulator::contains(const std::vector<int> vect, int val){
  return end(vect) != std::find(begin(vect), end(vect), val);
}

void particle::Simulator::push_dir(double d) {
  double ang;
  std::vector<int> okTraj;
  std::vector<int> okTraj2;
  std::vector<int> pbTraj;
  int myDir (0);
  double myDist(std::numeric_limits<double>::infinity());

/*
  // Target reset ?
  if(target == Point(0,0)){
    for (int i=myRect.size() ; i < myElements.size() ; i++) {
      myElements[i]->speed.x = 0;
      myLock[i].first = 0;
      myLock[i].second = Point(0,0);
    }
  }
  else{
    // Loop on all UAVs
    for (int i=myRect.size() ; i < myElements.size() ; i++) {
      // Target reached
      bool bReach((myElements[i]->pos-target).norm() <= d + sqrt(myElements[i]->getR2()));
      if (bReach){
        //Already just reached ?
        if(!(myElements[i]->speed.x == 0)){
          std::cout << "[" << i << "] : Target reached !" << std::endl;
        }
        okTraj.push_back(i);
        myElements[i]->speed.x = 0;
      }
      // Target not reached
      else{
        // Target on sight
        bool bTsight (particle::Simulator::checkTraj(myElements[i]->pos,sqrt(myElements[i]->getR2()),target,d,0.1));
        if(bTsight){
          okTraj.push_back(i);
          myElements[i]->speed.x = vmax;
          // First time that target is on sight
          if (!(myLock[i].second == target)) {
            std::cout << "[" << i << "] : Target locked !" << std::endl;
            // Align elmnt
            particle::Simulator::align(i,target);
            myLock[i].first = 0;
            myLock[i].second = target;
          }
          // Create okTraj2
          for (int j=myRect.size() ; j < myElements.size() ; j++) {
            bool bTsight2(particle::Simulator::checkTraj(myElements[i]->pos,sqrt(myElements[i]->getR2()),myElements[j]->pos,sqrt(myElements[j]->getR2()),0.1));
            if((i!=j) && bTsight2 && !particle::Simulator::contains(okTraj2,j)){
              okTraj2.push_back(j);
            }
          }
        }
        // Target not on sight
        else{
          // count UAV on sight
          int cnt(0);
          for (int j=myRect.size() ; j < myElements.size() ; j++) {
            // Different UAV
            if(i!=j){
              // UAV on sight
              bool bUsight (particle::Simulator::checkTraj(myElements[i]->pos,sqrt(myElements[i]->getR2()),myElements[j]->pos, sqrt(myElements[j]->getR2()), 0.1));
              if(bUsight){cnt++;}
            }
          }
          // UAV on sight
          if (cnt>0){
            pbTraj.push_back(i);
          }
          // no UAV on sight
          else{
            // Nothing locked
            bool noLock(myLock[i].first == 0 && myLock[i].second == Point(0,0));
            if(noLock){
              myElements[i]->speed.x = 0;
              particle::Simulator::explore(i,d);
            }
            // Something locked
            else{
              myElements[i]->speed.x = vmax;
            }
          }
        }
      }
    }
    // Loop on those that have a UAV on sight
    for (int i = 0 ; i < pbTraj.size() ; i++){
      int myDir (0);
      double myDist(std::numeric_limits<double>::infinity());
      // If 2nd gen
      if (particle::Simulator::contains(okTraj2,pbTraj[i])){
        // Best option
        for(int j = 0 ; j < okTraj.size() ; j++){
          bool bTsight = particle::Simulator::checkTraj(myElements[pbTraj[i]]->pos,sqrt(myElements[pbTraj[i]]->getR2()),myElements[okTraj[j]]->pos,sqrt(myElements[okTraj[j]]->getR2()),0.1);
          if(bTsight && ((target-myElements[okTraj[j]]->pos).norm() < myDist)){
            myDir = okTraj[j];
            myDist = (target-myElements[okTraj[j]]->pos).norm();
          }
        }
        particle::Simulator::align(pbTraj[i],myElements[myDir]->pos);
        myElements[pbTraj[i]]->speed.x = vmax;
        if (!(myLock[pbTraj[i]].first == myDir)){
          std::cout << "[" << pbTraj[i] << "] : Redirecting towards [" << myDir << "]" << std::endl;
          myLock[pbTraj[i]].first = myDir;
        }
        myLock[pbTraj[i]].second = particle::Point(0,0);
      }
      // Further
      else{
        // find 2nd gen
        for(int j = 0 ; j < okTraj2.size() ; j++){
          if((target-myElements[okTraj2[j]]->pos).norm() < myDist){
            myDir = j;
            myDist = (target-myElements[okTraj2[j]]->pos).norm();
          }
        }
        // if 2nd gen on sight
        if(myDir > 0){
          particle::Simulator::align(pbTraj[i],myElements[myDir]->pos);
          myElements[pbTraj[i]]->speed.x = vmax;
          // Already locked ?
          if (!(myLock[pbTraj[i]].first == myDir)){
            std::cout << "[" << pbTraj[i] << "] : Redirecting towards [" << myDir << "]" << std::endl;
            myLock[pbTraj[i]].first = myDir;
          }
          myLock[pbTraj[i]].second = particle::Point(0,0);
        }
        // too far
        else{
          // no locked target ?
          if(myLock[pbTraj[i]].first == 0 && myLock[pbTraj[i]].second == Point(0,0)){

            myElements[pbTraj[i]]->speed.x = 0;
            particle::Simulator::explore(pbTraj[i],d);
          }
          // already locked
          else{
            myElements[pbTraj[i]]->speed.x = vmax;
          }
        }
      }
    }

  }*/

  // Loop on UAVs
  for (int i=myRect.size() ; i < myElements.size() ; i++) {
    // Target not set ?
    if(target == Point(0,0)){
      myElements[i]->speed.x = 0;
      myLock[i].first = 0;
      myLock[i].second = Point(0,0);
    }
    else{
      // Target reached
      if((myElements[i]->pos-target).norm() <= d + sqrt(myElements[i]->getR2())){
        // Not at target before
        if (!(myElements[i]->speed.x == 0)){
          std::cout << "[" << i << "] : Target reached!" << std::endl;
        }
        okTraj.push_back(i);
        myElements[i]->speed.x = 0;
      }
      // Target not reached yet
      else{
        // Target on sight
        if (particle::Simulator::checkTraj(myElements[i]->pos,sqrt(myElements[i]->getR2()),target,d,0.1)){
          okTraj.push_back(i);
          myElements[i]->speed.x = 0.5*vmax;
          // Different target than previously
          if (!(myLock[i].second == target)) {
            std::cout << "[" << i << "] : Target locked!" << std::endl;
            // Align elmnt
            particle::Simulator::align(i,target);
            myLock[i].first = 0;
            myLock[i].second = target;
          }
        }
        else {pbTraj.push_back(i);}
      }
    }
  } 

 // Adjust traj for those with obstructed traj
 for (int i = 0 ; i < pbTraj.size() ; i++){
  //Loop to find closest OK traj
  myDir = 0;
  for(int j = 0 ; j < okTraj.size() ; j++){
    //Check if reachable
    bool uavReach;
    uavReach = particle::Simulator::checkTraj(myElements[pbTraj[i]]->pos,sqrt(myElements[pbTraj[i]]->getR2()),myElements[okTraj[j]]->pos, sqrt(myElements[okTraj[j]]->getR2()), 0.1);
    if (uavReach){
      //Check if closer
      if((myElements[pbTraj[i]]->pos-myElements[okTraj[j]]->pos).norm() < myDist){
        myDir = okTraj[j];
      }
    }
  }
  //Align
  if (myDir >0){ 
    myElements[pbTraj[i]]->speed.x = vmax;
    particle::Simulator::align(pbTraj[i],myElements[myDir]->pos);
    if(!(myLock[pbTraj[i]].first == myDir)){ // check if not already locked on UAV
      std::cout << "[" << pbTraj[i] << "] : Redirecting towards [" << myDir << "]" << std::endl;
      myLock[pbTraj[i]].first = myDir;
      myLock[pbTraj[i]].second = particle::Point(0,0);
    }
  }
  // no OK traj reachable
  else {
    // no locked target ?
    if(myLock[pbTraj[i]].first == 0 && myLock[pbTraj[i]].second == Point(0,0)){
      myElements[pbTraj[i]]->speed.x = 0;
      particle::Simulator::explore(pbTraj[i],d);
    }
    else{
      myElements[pbTraj[i]]->speed.x = vmax;
    }
  }
 }
}


particle::Point particle::Simulator::getObst(const particle::Point& pos, double radius, double ang, double precision){
  particle::Point shot,v(Point(cos(ang),sin(ang)));
  for (int i=0 ; i < (int)(500/precision); i++){
    //update shot
    shot = pos + (radius + i*precision)*v;
    // check each Rect
    for (int j=0 ; j < myRect.size() ; j++){
      particle::Prism* pr = myElements[j]->make_prism();
      if (pr->belongs(shot,0)){
        shot = shot - precision*v;
        return shot;
      }
    }
  }
  return Point(0,0);
}

void particle::Simulator::explore(int i, double d){
  double ang(0);
  double samples(1000);
  const double PI = 3.141592653589793;
  // loop on angle
  particle::Point obst,prevObst,targetbis(0,0);
  double theDist = std::numeric_limits<double>::infinity();
  // Not explored yet
  if (myExplo[i] == false){
    std::cout << "[" << i << "] : Looking for openings from (" << myElements[i]->pos.x << "," << myElements[i]->pos.y << ")" << std::endl;
    // Going through all angles
    for (int j=0; j < samples; j++){
      ang = j*2*PI/samples;
      obst = particle::Simulator::getObst(myElements[i]->pos, sqrt(myElements[i]->getR2()), ang, 0.1);
      if (j>0){

        //Closest between obst and prevobst
        particle::Point min,max;
        if ((obst-myElements[i]->pos).norm() > (prevObst-myElements[i]->pos).norm()){
          min = prevObst;
          max = obst;
        }
        else{
          min = obst;
          max = prevObst;
        }

        //discontinuity
        bool disc,okBeyond;
        particle::Point checkPt;
        disc = (max-min).norm() > 8*PI*(min-myElements[i]->pos).norm()/samples;
        checkPt = min + 5*(min-myElements[i]->pos)/(min-myElements[i]->pos).norm();
        okBeyond = checkTraj(min, 1, checkPt, 0.1, 0.1);
        if(disc && !(obst == Point(0,0)) && okBeyond){
          // Better than previous discontinuity ?
          double path;
          path = (target-min).norm();
          //std::cout << "Discontinuity spotted at (" << min.x << "," << min.y << ") at " << path << " from target" << std::endl;
          //std::cout << "Checking traj between (" << min.x << "," << min.y << ") and (" << checkPt.x << "," << checkPt.y << ") : " << okBeyond << std::endl;
          if (path < theDist){
            //Update target and dist
            theDist = path;
            targetbis = min;
          }
        }
      }
      prevObst = obst;
    }
    // Opening found
    if (!(targetbis == Point(0,0))){
      std::cout << "[" << i << "] : Opening at (" << targetbis.x << "," << targetbis.y << ")" << std::endl;
      particle::Simulator::align(i,targetbis);
      std::cout << "[" << i << "] : Aligning with angle " << myElements[i]->angle/PI << " * PI" << std::endl;
      myElements[i]->speed = Point(vmax,0);
      myLock[i].first = 0;
      myLock[i].second = targetbis;
    }
    // Exploration unsuccessfull
    else{
      myExplo[i] = true;
      std::cout << "[" << i << "] : No opening found." << std::endl;
    }
  }
}

void particle::Simulator::push_target(const particle::Point& t){
  target = t;
  for (int i=0 ; i < myLock.size() ; i++) {
    myLock[i].first = 0;
    myLock[i].second = Point();
    myExplo[i] = false;
  }
}
