#include "Catcher.h"
#include "World.h"


int getDistance(Point2D start, Point2D end) 
{
  //from https://www.redblobgames.com/grids/hexagons/#conversions
  int parity = start.y % 2;
  int sq = start.x - (start.y - parity) / 2;
  int sr = start.y;

  parity = end.y % 2;
  int eq = end.x - (end.y - parity) / 2;
  int er = end.y;

  Point2D vector = {sq - eq, sr - er};
  int output = (std::abs(vector.x) + std::abs(vector.x + vector.y) + std::abs(vector.y)) / 2;
  return output;
}

//make a hexagon shaped net around the cat and fill in the weakest links
//hexagon side size is the exact same as world side size
Point2D Catcher::Move(World* world) {
  auto ssO2 = world->getWorldSideSize() / 2;
  Point2D cat = world->getCat();

  //the hexagon
  turns[0] = {ssO2, 0};
  turns[1] = {ssO2 - ssO2 / 2, ssO2};
  turns[2] = {-ssO2 + ssO2 / 2, ssO2};
  turns[3] = {-ssO2, 0};
  turns[4] = {-ssO2 + ssO2 / 2, -ssO2};
  turns[5] = {ssO2 - ssO2 / 2, -ssO2};

  //this is gonna orient us while we loop through the hexagon
  int direction = 0;
  Point2D checking = world->SW(turns[0]);
  Point2D output = turns[0];

  int outputDis;
  if (world->getContent(turns[0]))
  {
    outputDis = ssO2 * 4;
  }
  else {
    outputDis = getDistance(cat, turns[0]);
  }
  //starter plan - find the closest unfilled point on the hexagon
  while (checking != turns[0])
  {
    int newDis = getDistance(cat, checking);
    if (newDis < outputDis && !world->getContent(checking) && newDis != 0)
    {
        output = checking;
        outputDis = newDis;
    }
    //look at next hex
    switch (direction) {
      case 0:
        checking = world->SW(checking);
        if (checking == turns[direction + 1]) { direction++; }
        break;
      case 1:
        checking = world->W(checking);
        if (checking == turns[direction + 1]) { direction++; }
        break;
      case 2:
        checking = world->NW(checking);
        if (checking == turns[direction + 1]) { direction++; }
        break;
      case 3:
        checking = world->NE(checking);
        if (checking == turns[direction + 1]) { direction++; }
        break;
      case 4:
        checking = world->E(checking);
        if (checking == turns[direction + 1]) { direction++; }
        break;
      case 5:
        checking = world->SE(checking);
        break;
      default:
        break;
    }
  }

  if (world->getContent(output))
  {
    while (world->getContent(output)) {
      switch (direction) {
        case 0:
          output = world->SW(cat);
          break;
        case 1:
          output = world->W(cat);
          break;
        case 2:
          output = world->NW(cat);
          break;
        case 3:
          output = world->NE(cat);
          break;
        case 4:
          output = world->E(cat);
          break;
        case 5:
          output = world->SE(cat);
          break;
        default: break;
      }
      direction++;
      direction = direction % 6;
    }
  }

  return output;

  //should not be reached
  for (;;) {
    Point2D p = {Random::Range(-ssO2, ssO2), Random::Range(-ssO2, ssO2)};
    if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  }
}
