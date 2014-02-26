#ifndef LIBESOM_SOM_H
#define LIBESOM_SOM_H

#include <vector>

namespace esom {
  class BestMatchFunction {
  public:
    virtual void init(Grid&) {}
    virtual void update(Grid&) {}
    virtual int operator() (Vector) = 0;
  };

  class BaseSOM {
  protected:
    Grid& grid;
    BestMatchFunction& bestMatch;
    NeighbourhoodFunction& neighbourhood;
    CoolingFunction& cooling;
    CoolingFunction& radiusCooling;

    int step, epoch;
  public:
    BaseSOM(Grid& grid,
	    BestMatchFunction& bestMatch,
	    NeighbourhoodFunction& neighbourhood,
	    CoolingFunction& cooling,
	    CoolingFunction& radiusCooling);
    virtual void train(Matrix) = 0;
    virtual void train(Vector) = 0;
    virtual void init() = 0;
    virtual void endEpoch() { epoch++; }
  };

  class OnlineSOM : public BaseSOM {
    void updateModel(int i, int c, Vector x);
    void update(Vector x);
    double radius, alpha;
  public:
    OnlineSOM(Grid& grid,
	      BestMatchFunction& bestMatch,
	      NeighbourhoodFunction& neighbourhood,
	      CoolingFunction& cooling,
	      CoolingFunction& radiusCooling);
    virtual void init();
    virtual void train(Matrix);
    virtual void train(Vector);
    virtual void endEpoch();
  };
}

#endif
