#ifndef LIBESOM_COOLING_H
#define LIBESOM_COOLING_H

namespace esom {
  namespace cooling {
    class Linear : public CoolingFunction {
      double max, min, steps;
    public:
      Linear(double max, double min, int steps) 
	: max(max), min(min), steps(steps) {}
      virtual double operator() (int step) {
	return max - (max-min)*((double)step / steps);
      }
    };
  }
}

#endif
