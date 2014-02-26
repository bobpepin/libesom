#include <vector>
#include "Data.h"
#include "ESOM.h"
#include "Grid.h"
#include "UMatrix.h"

namespace esom {
  UMatrix::UMatrix(DistanceFunction& distance, Grid& grid) : 
    distance(distance), grid(grid) {
    values = new double[grid.size()];
  }

  UMatrix::~UMatrix() {
    delete[] values;
  }

  void UMatrix::calculate() {
    for(unsigned int i=0; i < size(); i++) {
      values[i] = calculatePoint(i);
    }
  }

  double UMatrix::calculatePoint(unsigned int i) {
    std::vector<unsigned int> neighbours = grid.neighbours(i);
    std::vector<unsigned int>::iterator iter;
    double sum = 0;
    for(iter = neighbours.begin(); iter != neighbours.end(); iter++) {
      unsigned int j = *iter;
      sum += distance(grid.getModel(i), grid.getModel(j));
    }
    return sum / neighbours.size();
  }
}
