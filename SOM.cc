#include "Data.h"
#include "ESOM.h"
#include "Grid.h"
#include "BestMatch.h"
#include "SOM.h"

#include <cstdlib>
#include <limits>
#include <algorithm>

namespace esom {
  BaseSOM::BaseSOM(Grid& grid,
		   BestMatchFunction& bestMatch,
		   NeighbourhoodFunction& neighbourhood,
		   CoolingFunction& cooling,
		   CoolingFunction& radiusCooling) 
    : grid(grid), 
      bestMatch(bestMatch), 
      neighbourhood(neighbourhood),
      cooling(cooling), 
      radiusCooling(radiusCooling),
      step(0), epoch(0) {}

  OnlineSOM::OnlineSOM(Grid& grid,
		       BestMatchFunction& bestMatch,
		       NeighbourhoodFunction& neighbourhood,
		       CoolingFunction& cooling,
		       CoolingFunction& radiusCooling) 
    : BaseSOM(grid, bestMatch, neighbourhood, cooling, radiusCooling) {
    alpha = cooling(epoch);
    radius = radiusCooling(epoch);
  };

  void OnlineSOM::init() {
    for(unsigned int i=0; i < grid.size(); i++) {
      Vector m = grid.getModel(i);
      Vector::iterator iter;
      for(iter = m.begin(); iter != m.end(); iter++) {
	double n = (double)std::rand() / RAND_MAX;
	*iter = -1 + 2*n;
      }
    }
    bestMatch.init(grid);
  }
  
  void OnlineSOM::endEpoch() {
    BaseSOM::endEpoch();
    alpha = cooling(epoch);
    radius = radiusCooling(epoch);
  }

  void OnlineSOM::train(Vector x) {
#if 0
    for(Vector::iterator i = x.begin(); i != x.end(); i++) {
      std::cerr << *i << "\t";
    }
    std::cerr << std::endl;
#endif
    update(x);
    step++;
  }

  void OnlineSOM::train(Matrix data) {
    for(unsigned int i=0; i < data.rows(); i++) {
      train(data(i));
    }
  }

  void OnlineSOM::updateModel(int i, int c, Vector x) {
    assert(x.size() == grid.dimension());
    Vector m = grid.getModel(i);
    double *dm1 = new double[m.size()];
    Vector dm(dm1, m.size());
    std::copy(x.begin(), x.end(), dm.begin());

    dm -= m;
    // double alpha = cooling(epoch);
    // double radius = radiusCooling(epoch);

    double d2 = grid.distance2(c, i);
    double h = neighbourhood(d2, radius);
    dm *= (h*alpha);
    m += dm;
    delete[] dm1;
  }

  void OnlineSOM::update(Vector x) {
    int c = bestMatch(x);
    double radius = radiusCooling(epoch);
    std::vector<unsigned int> nodes = grid.inRadius(c, radius);
    //    std::cerr << nodes.size() << " nodes in radius " << radius << std::endl;
    for(unsigned int i=0; i < grid.size(); i++) {
      updateModel(i, c, x);
      bestMatch.update(grid);
    }
  }

}
