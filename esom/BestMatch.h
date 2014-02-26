#ifndef LIBESOM_BESTMATCH_H
#define LIBESOM_BESTMATCH_H

#include "ESOM.h"
#include "SOM.h"

#include <limits>

namespace esom {
  namespace bestmatch {
    class Linear : public BestMatchFunction {
      Grid* grid;
    public:
      DistanceFunction& distance;
    Linear(DistanceFunction& distance) : distance(distance) {}

      void init(Grid& g) {
	grid = &g;
      }

      virtual int operator() (Vector input) {
	double min = std::numeric_limits<double>::max();
	int min_i = 0;
	for(size_t i=0; i < grid->size(); i++) {
	  Vector model = grid->getModel(i);
	  double d = distance(model, input);
	  if(d < min) {
	    min = d;
	    min_i = i;
	  }
	}
	//	std::cerr << "best match = " << min_i << " distance = " << min << std::endl;
	return min_i;
      }
    };
  }
}

#endif
