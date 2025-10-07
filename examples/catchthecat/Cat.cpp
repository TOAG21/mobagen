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
  if (world->isValidPosition(World::NE(pos))) {
    if (world->getContent(World::NE(pos))) {
      output++;
    }
  }
  if (world->isValidPosition(World::E(pos))) {
    if (world->getContent(World::E(pos))) {
      output++;
    }
  }
  if (world->isValidPosition(World::SE(pos))) {
    if (world->getContent(World::SE(pos))) {
      output++;
    }
  }
  if (world->isValidPosition(World::SW(pos))) {
    if (world->getContent(World::SW(pos))) {
      output++;
    }
  }
  if (world->isValidPosition(World::W(pos))) {
    if (world->getContent(World::W(pos))) {
      output++;
    }
  }
  if (world->isValidPosition(World::NW(pos))) {
    if (world->getContent(World::NW(pos))) {
      output++;
    }
  }
  return output;
}

void addSortIndex(std::vector<int>& array, Location& newL, std::unordered_map<int, Location>& exploring, int ss) {
  for (int i = 0; i < array.size(); i++) 
  {
      if (newL.costEstimated < exploring.at(array[i]).costEstimated)
      {
        array.insert(array.begin() + i, toIndex(newL.point, ss));
        return;
      }
  }
  array.push_back(toIndex(newL.point, ss));
}

//the cat should calculate distance + blocks in path
//add neighborhood to heuristic
Point2D Cat::Move(World* world) {
  auto rand = Random::Range(0, 5);
  auto pos = world->getCat();
  int ss = world->getWorldSideSize();

  std::vector<int> toSearch;
  toSearch.push_back(toIndex(pos, ss));
  std::unordered_map<int, Location> exploring;
  std::unordered_map<int, Location> explored;
  exploring.insert({toIndex(pos, ss), Location(pos, pos, 0, 0)});

  Location current;
  std::vector<Location> neighbors;
  Location goal;
  goal.costSoFar = 5000;

  while (exploring[toSearch[0]].costEstimated < goal.costSoFar) {
    current = exploring[toSearch[0]];

    // get all neighbors of current
    if (world->isValidPosition(World::NE(current.point))) {
        neighbors.push_back(Location(World::NE(current.point), current.point, 0, 0));
    }
    if (world->isValidPosition(World::E(current.point))) {
        neighbors.push_back(Location(World::E(current.point), current.point, 0, 0));
    }
    if (world->isValidPosition(World::SE(current.point))) {
        neighbors.push_back(Location(World::SE(current.point), current.point, 0, 0));
    }
    if (world->isValidPosition(World::SW(current.point))) {
        neighbors.push_back(Location(World::SW(current.point), current.point, 0, 0));
    }
    if (world->isValidPosition(World::W(current.point))) {     
        neighbors.push_back(Location(World::W(current.point), current.point, 0, 0));
    }
    if (world->isValidPosition(World::NW(current.point))) {
        neighbors.push_back(Location(World::NW(current.point), current.point, 0, 0));
    }

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

      int index = toIndex(loc.point, ss);
      // if its within searched
      if (explored.contains(index))
      {
          // check if we found a better route
          if (explored[index].costSoFar <= loc.costSoFar)
          {
              // not better route then leave it
              continue;              
          }
          // better route move it back to exploring
          exploring.insert({index, explored[index]});
          //toSearch.push(index);
          addSortIndex(toSearch, loc, exploring, ss);
          explored.erase(index);
          
          exploring[index].costSoFar = loc.costSoFar;
          exploring[index].fromPoint = loc.fromPoint;
      }
      // if its already in exploring
      else if (exploring.contains(index))
      {
        // check if we found a better route
        if (exploring[index].costSoFar <= loc.costSoFar) {
          // not better route then leave it
          continue;
        }
        // better route simply record that
        exploring[index].costSoFar = loc.costSoFar;
        exploring[index].fromPoint = loc.fromPoint;
      }
      else
      {
          //add location to exploring
          exploring.insert({index, loc});
          //add index to to search
          addSortIndex(toSearch, loc, exploring, ss);
      }
    }

    //once were done looking at connections
    //add it to explored
    explored.insert({toIndex(current.point, ss), current});
    //remove it from exploring 
    exploring.erase(toIndex(current.point, ss));
    //remove it from to search
    toSearch.erase(toSearch.begin());
    //check if the goal has been improved
    if (std::abs(current.point.x) == ss/2 || std::abs(current.point.y) == ss/2)
    {
        if (current.costSoFar < goal.costSoFar)
        {
            goal = current;
        }
    }
  }

  //we should now have the most effecient route to border
  // get the trace back from the goal location
  Point2D checkAt = goal.point;
  Point2D checkTo = goal.fromPoint;
  while (checkTo != pos)
  {
    checkAt = checkTo;
    checkTo = explored[toIndex(checkTo, ss)].fromPoint;
  }
  //output the node that connects to our current position
  return checkAt;


  //shouldnt hit this. delete later
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
