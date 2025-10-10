#include "Catcher.h"
#include "World.h"

Fiber::Fiber() {}
Fiber::Fiber(int nIn, int fIn, int cIn) {
  netStrength = nIn;
  fromIndex = fIn;
  catDist = cIn;
}

static int toIndex(Point2D point, int sideSize) 
{ 
    return ((point.y + sideSize / 2) * sideSize + (point.x + sideSize / 2)); 
}
static Point2D toPoint(int pos, int sideSize) 
{
  bool sign = pos >= 0;
  int x = pos % sideSize - sideSize / 2;
  int y = (pos / sideSize) - sideSize / 2;
  return {x, y};
}

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
int getFiberDistance(Point2D start, Point2D end, int ssO2) 
{ 
    int catcherCautionR = 6;
    int output = getDistance(start, end);
    if (output == 0) {  return -50;   }
    if (output > catcherCautionR) 
  {
      output = catcherCautionR;
  }
    output = catcherCautionR - output;
  output = output * 4;
  return output;
}

int getStrength(World* world, std::unordered_map<int, Fiber>& net, int ssO2, Point2D hex, int fromI)
{ 
    int output = 0;
    if (world->getContent(hex))
    {
      return output;  
    }
    output = net.at(fromI).netStrength + 1;
    //output += getDistance(world->getCat(), hex) % ssO2;
    return output;
}

Fiber recursiveStrengthCalc(World* world, std::unordered_map<int, Fiber>& net, int base, int fromI, Fiber output) 
{
  Fiber& focus = net.at(fromI);
  if (focus.fromIndex == fromI)
  {
    return output;
  }
  Fiber& from = net.at(base);
  int strBonus = 1;

  if (from.netStrength > 0) {    strBonus++;  }
  if (net.at(focus.fromIndex).netStrength > 0) {  strBonus++;  }

  if (focus.netStrength > from.netStrength + strBonus) 
  {
    focus.netStrength = from.netStrength + strBonus;
  }
  if (focus.netStrength != 0 && (focus.netStrength + focus.catDist > output.netStrength + output.catDist || output.netStrength == 0)) {
    output.netStrength = focus.netStrength;
    output.catDist = focus.catDist;
    output.fromIndex = fromI;
  }
  output = recursiveStrengthCalc(world, net, fromI, focus.fromIndex, output);
  return output;
}

//make a hexagon shaped net around the cat and fill in the weakest links
//hexagon side size is the exact same as world side size
Point2D Catcher::Move(World* world) {
  auto ss = world->getWorldSideSize();
  auto ssO2 = ss / 2;
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
    return turns[0];
  }

  int fromI = toIndex(turns[0], ss);
  Fiber outputF = Fiber(0, fromI, getFiberDistance(cat, turns[0], ssO2));
  net[fromI] = Fiber(0, fromI, getFiberDistance(cat, turns[0], ssO2));
  int str = 0;

  //starter plan - find the closest unfilled point on the hexagon
  while (checking != turns[0])
  {
    str = getStrength(world, net, ssO2, checking, fromI);
    net[toIndex(checking, ss)] = Fiber(str, fromI, getFiberDistance(cat, checking, ssO2));

    fromI = toIndex(checking, ss);

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

  //recursive back loop to find highest net weakness
  outputF = recursiveStrengthCalc(world, net, toIndex(turns[0], ss), fromI, outputF);
  output = toPoint(outputF.fromIndex, ss);

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
}
