#include "Cat.h"
#include "World.h"
#include <stdexcept>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>

struct Location
{
    Point2D point;
    Point2D fromPoint;
    int costSoFar;
    int costEstimated;

    Location() {}
    Location(Point2D pIn, Point2D fromIn, int csfIn, int ceIn)
    { 
      point = pIn;
      fromPoint = fromIn;
      costSoFar = csfIn;
      costEstimated = ceIn;
    }
};

struct MinHeapComp
{
    bool operator()(Location one, Location two)
    { 
        return one.costEstimated > two.costEstimated;
    }
};


//the cat should calculate distance + blocks in path
//add neighborhood to heuristic
Point2D Cat::Move(World* world) {
  auto rand = Random::Range(0, 5);
  auto pos = world->getCat();

  //change this to unordered hash
  std::priority_queue<Location, std::vector<Location>, MinHeapComp> toSearch;
  toSearch.push(Location(pos, pos, 0, 0));
  std::priority_queue<Location, std::vector<Location>, MinHeapComp> searched;

  Location current;
  Location goal;
  goal.costSoFar = 50;

  while (toSearch.top().costEstimated < goal.costSoFar)
  {
    current = toSearch.top();
    //get all neighbors of current
    //for each {
    //get cost so far (distance + blocked neighbors)
    //estimate final cost (shortest route to border)
    // 
    //if its within searched
    //check if we found a better route
    //not better route then leave it
    //better route move it back to tosearch
    // 
    //if its already in to search
    // check if we found a better route
    // not better route then leave it
    // better route simply record that
    //
    //otherwise we need to add it to open


  }

  switch (rand) {
    case 0:
      return World::NE(pos);
    case 1:
      return World::NW(pos);
    case 2:
      return World::E(pos);
    case 3:
      return World::W(pos);
    case 4:
      return World::SW(pos);
    case 5:
      return World::SE(pos);
    default:
      throw "random out of range";
  }
}
