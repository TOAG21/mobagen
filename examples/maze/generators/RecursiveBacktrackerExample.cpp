#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
bool RecursiveBacktrackerExample::Step(World* w) {
  int choice;

  if (stack.empty())
  {
      return false;
  }

  std::vector<Point2D> visitables = getVisitables(w, stack.back());
  visited[stack.back().x][ stack.back().y] = true;
  w->SetNodeColor(stack.back(), Color::Gold);
  if (visitables.size() > 0)
  {
      if (visitables.size() == 1)
      {
          choice = 0;
      }
      else
      {
        choice = rand() % visitables.size();
      }

      switch (direction[choice]) {
        case 0:
          w->SetSouth(stack.back(), false);
          break;
        case 1:
          w->SetEast(stack.back(), false);
          break;
        case 2:
          w->SetNorth(stack.back(), false);
          break;
        case 3:
          w->SetWest(stack.back(), false);
          break;
        default:
          break;
      }

      stack.push_back(visitables[choice]);
      w->SetNodeColor(stack.back(), Color::White);
  }
  else
  {
    w->SetNodeColor(stack.back(), Color::Black);
    stack.pop_back();
  }


  return true;
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
  stack.push_back( randomStartPoint(world));
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  direction.clear();
  if (p.y + 1 <= sideOver2) 
  {
    if (!visited[p.x][p.y + 1]) 
    {
      visitables.push_back(Point2D(p.x, p.y + 1));
      direction.push_back(0);
    }
  }
  if (p.x + 1 <= sideOver2) 
  {
    if (!visited[p.x + 1][p.y]) 
    {
      visitables.push_back(Point2D(p.x + 1, p.y));
      direction.push_back(1);
    }
  }
  if (p.y - 1 >= -1 * sideOver2) 
  {
    if (!visited[p.x][p.y - 1]) 
    {
      visitables.push_back(Point2D(p.x, p.y - 1));
      direction.push_back(2);
    }
  }
  if (p.x - 1 >= -1 * sideOver2) 
  {
    if (!visited[p.x - 1][p.y]) 
    {
      visitables.push_back(Point2D(p.x - 1, p.y));
      direction.push_back(3);
    }
  }

  return visitables;
}
