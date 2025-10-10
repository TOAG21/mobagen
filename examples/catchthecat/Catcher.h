#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"
#include <queue>
#include <vector>
#include <functional>

struct Fiber 
{
  int netStrength;
  int fromIndex;
  int catDist;

  Fiber();
  Fiber(int nIn, int fIn, int cIn);  
};

class Catcher : public Agent {
public:
  explicit Catcher() : Agent(){};
  Point2D Move(World*) override;

private:
  Point2D turns[6];
  Point2D catLast = {0, 0};

  std::unordered_map<int, Fiber> net;
};

#endif  // CATCHER_H
