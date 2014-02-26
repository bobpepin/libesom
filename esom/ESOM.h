#ifndef LIBESOM_ESOM_H
#define LIBESOM_ESOM_H

namespace esom {
  class DistanceFunction {
  public:
    virtual double operator() (Vector, Vector) = 0;
  };

  class NeighbourhoodFunction {
  public:
    virtual double operator() (int distance2, double radius) = 0;
  };

  class CoolingFunction {
  public:
    virtual double operator() (int step) = 0;
  };
  
}

#endif
