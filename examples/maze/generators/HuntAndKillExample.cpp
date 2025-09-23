#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>
bool HuntAndKillExample::Step(World* w) {
    std::vector<Point2D> visitables;
    if (!stack.empty())
    {
      visitables = getVisitables(w, stack.back());
      visited[stack.back().x][stack.back().y] = true;
      w->SetNodeColor(stack.back(), Color::Black);
      int choice = -1;
      if (visitables.size() == 1)
      {
        choice = 0;
      }
      else if (visitables.size() > 1) 
      {
        choice = rand() % visitables.size();
      }
      else if (visitables.size() <= 0)
      {
          //go back to hunting
        stack.clear();
        return true;
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
        //hunt
      bool foundSpace = false;
      while (!foundSpace)
      {
        visitables = getVisitables(w, huntLocation);
        if (visitables.size() > 0)
        {
          stack.push_back(huntLocation);
          w->SetNodeColor(stack.back(), Color::White);
          foundSpace = true;
        } 
        else 
        {
            int sideOver2 = w->GetSize() / 2;
            if (huntLocation.x + 1 > sideOver2)
            {
                if (huntLocation.y + 1 > sideOver2)
                {
                    return false;
                }
                huntLocation.x = -sideOver2;
                huntLocation.y++;
            }
            else
            {
                huntLocation.x++;
            }
        }
      }
    }

  return true;
}
void HuntAndKillExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }

  huntLocation = Point2D(-sideOver2, -sideOver2);
  stack.push_back(randomStartPoint(world));
}
Point2D HuntAndKillExample::randomStartPoint(World* world) {
  // Todo: improve this if you want
  auto sideOver2 = world->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  
  direction.clear();
  if (p.y + 1 <= sideOver2) {
    if (!visited[p.x][p.y + 1]) {
      visitables.push_back(Point2D(p.x, p.y + 1));
      direction.push_back(0);
    }
  }
  if (p.x + 1 <= sideOver2) {
    if (!visited[p.x + 1][p.y]) {
      visitables.push_back(Point2D(p.x + 1, p.y));
      direction.push_back(1);
    }
  }
  if (p.y - 1 >= -1 * sideOver2) {
    if (!visited[p.x][p.y - 1]) {
      visitables.push_back(Point2D(p.x, p.y - 1));
      direction.push_back(2);
    }
  }
  if (p.x - 1 >= -1 * sideOver2) {
    if (!visited[p.x - 1][p.y]) {
      visitables.push_back(Point2D(p.x - 1, p.y));
      direction.push_back(3);
    }
  }

  return visitables;
}
std::vector<Point2D> HuntAndKillExample::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  // todo: code this

  return neighbors;
}
