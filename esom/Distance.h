#ifndef LIBESOM_DISTANCE_H
#define LIBESOM_DISTANCE_H

#include <cmath>
#include <cfloat>

namespace esom {
  namespace distance {
    class Euclidean : public DistanceFunction {
    public:
      virtual double operator() (Vector a, Vector b) {
	double norm2 = 0;
	Vector::const_iterator ai, bi;
	assert(a.size() == b.size());
	for(ai = a.begin(), bi = b.begin(); ai < a.end(); ai++, bi++) {
	  double d = *bi - *ai;
	  norm2 += d * d;
	}
	return std::sqrt(norm2);
      }
    };

    class Correlation : public DistanceFunction {
    public:
      virtual double operator() (Vector a, Vector b) {
	assert(a.size() == b.size());
	unsigned int n = a.size();
	double x = 0, y = 0, xy = 0, x2 = 0, y2 = 0;
	for(unsigned int i=0; i < n; i++) {
	  double xi = a[i];
	  double yi = b[i];
	  x += xi;
	  y += yi;
	  xy += xi * yi;
	  x2 += xi * xi;
	  y2 += yi * yi;
	}
	double num = (n * xy - x * y);
	if(num == 0) {
	  return 0;
	}
	double denom = (n*x2 - x*x) * (n*y2 - y*y);
	double r = num / std::sqrt(denom);
	return 1 - r;
      }
    };
  }
}

#endif
