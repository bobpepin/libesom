#ifndef LIBESOM_UMATRIX_H
#define LIBESOM_UMATRIX_H

#include <vector>

namespace esom {
  class UMatrix : public GridOverlay {
    DistanceFunction& distance;
    Grid& grid;
    double* values;

    UMatrix& operator=(const UMatrix&);

  public:

    UMatrix(DistanceFunction& distance, Grid& grid);
    ~UMatrix();

    unsigned int size() {
      return grid.size();
    }
    unsigned int rows() {
      return grid.rows();
    }
    unsigned int columns() {
      return grid.columns();
    }
    unsigned int index2col(int i) {
      return grid.index2col(i);
    }
    unsigned int index2row(int i) {
      return grid.index2row(i);
    }
    unsigned int coords2index(int x, int y) {
      return grid.coords2index(x, y);
    }
    std::vector<unsigned int> neighbours(unsigned int i) {
      return grid.neighbours(i);
    }

    void calculate();

    double calculatePoint(unsigned int i);

    double getValue(unsigned int i) {
      return values[i];
    }
  };
}

#endif
