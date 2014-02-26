#ifndef LIBESOM_NEIGHBOURHOOD_H
#define LIBESOM_NEIGHBOURHOOD_H

namespace esom {
  namespace neighbourhood {
    class Gauss : public NeighbourhoodFunction {
      /* number of standard deviations corresponding to radius */
      int stddevs;

    public:

    Gauss(int stddevs = 2) : stddevs(stddevs) { }
      
      virtual double operator() (int distance2, double radius) {
	assert(distance2 >= 0);
	if(distance2 > (radius*radius)) {
	  return 0.0;
	}
	double norm = 2 * std::pow((double)(radius + 1), 2) / std::pow((double)stddevs, 2);
	//	std::cerr << "radius = " << radius << " stddevs = " << stddevs << " norm = " << norm << std::endl;
	return std::exp((double)(-distance2) / norm);
      }
    };
  }
}

#endif
