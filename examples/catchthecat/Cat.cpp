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

int toIndex(Point2D point, int sideSize) 
{ 
    return ((point.y + sideSize / 2) * sideSize + (point.x + sideSize / 2));
}
Point2D toPoint(int pos, int sideSize) {
  bool sign = pos >= 0;
  int x = pos % sideSize - sideSize / 2;
  int y = (pos / sideSize) - sideSize / 2;
  return {x, y};
}

int getBlockedNear(Point2D pos, World* world)
{ 
    int output = 0;
  if (world->getContent(World::NE(pos))) {
    output++;
  }
  if (world->getContent(World::E(pos))) {
    output++;
  }
  if (world->getContent(World::SE(pos))) {
    output++;
  }
  if (world->getContent(World::SW(pos))) {
    output++;
  }
  if (world->getContent(World::W(pos))) {
    output++;
  }
  if (world->getContent(World::NW(pos))) {
    output++;
  }
  return output;
}


//the cat should calculate distance + blocks in path
//add neighborhood to heuristic
Point2D Cat::Move(World* world) {
  auto rand = Random::Range(0, 5);
  auto pos = world->getCat();
  int ss = world->getWorldSideSize();

  //change this to unordered hash
  std::priority_queue<int, std::vector<int>, std::greater<int>> toSearch;
  toSearch.push(0);
  std::unordered_map<int, Location> exploring;
  std::unordered_map<int, Location> explored;
  exploring.insert({0, Location({0, 0}, {0, 0}, 0, 0)
});

  Location current;
  std::vector<Location> neighbors;
  Location goal;
  goal.costSoFar = 50;

  while (explored.at(toSearch.top()).costEstimated < goal.costSoFar)
  {
    current = explored.at(toSearch.top());

    // get all neighbors of current
    neighbors.push_back(Location(World::NE(current.point), current.point, 0, 0));
    neighbors.push_back(Location(World::E(current.point), current.point, 0, 0));
    neighbors.push_back(Location(World::SE(current.point), current.point, 0, 0));
    neighbors.push_back(Location(World::SW(current.point), current.point, 0, 0));
    neighbors.push_back(Location(World::W(current.point), current.point, 0, 0));
    neighbors.push_back(Location(World::NW(current.point), current.point, 0, 0));

    //for each {
    //get cost so far (distance + blocked neighbors)
    //estimate final cost (shortest route to border)
    for (Location loc : neighbors) 
    {
      loc.costSoFar = current.costSoFar + 1 + (2 * getBlockedNear(loc.point, world));
      int shortestDistance = 5, test;
      test = 5 - std::abs(loc.point.x);
      if (test < shortestDistance) {
        shortestDistance = test;
      }
      test = 5 - std::abs(loc.point.y);
      if (test < shortestDistance) {
        shortestDistance = test;
      }
      loc.costEstimated = loc.costSoFar + shortestDistance;

      // if its within searched
      if (explored.contains(toIndex(loc.point, ss)))
      {
        // check if we found a better route
        // not better route then leave it
        // better route move it back to exploring
      }
      // if its already in exploring
      else if (exploring.contains(toIndex(loc.point, ss)))
      {
        // check if we found a better route
        // not better route then leave it
        // better route simply record that
      }
      else
      {
          //add location to exploring
          //add index to to search
      }
    }

    //once were done looking at connections
    //add it to explored
    //remove it from exploring
    //remove it from to search

    //check if the goal has been improved

  }

  //we should now have the most effecient route to border
  // get the trace back from the goal location
  //output the node that connects to our current position


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
