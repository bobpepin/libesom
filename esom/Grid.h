#ifndef LIBESOM_GRID_H
#define LIBESOM_GRID_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

namespace esom {
  class Grid {
    unsigned int _rows, _columns, dim;

    // prevent copying
    Grid(const Grid&);
    Grid& operator=(const Grid&);
  public:
    double *models;
    Grid(unsigned int rows, unsigned int columns, unsigned int dim);
    ~Grid();

    unsigned int dimension() const {
      return dim;
    }
    unsigned int size() const {
      return rows()*columns();
    }
    unsigned int rows() const {
      return _rows;
    }
    unsigned int columns() const {
      return _columns;
    }
    
    virtual unsigned int index2col(int) const = 0;
    virtual unsigned int index2row(int) const = 0;
    virtual unsigned int coords2index(int, int) const = 0;
    virtual unsigned int distance2(int, int) const = 0;
    virtual bool isInsideGrid(int, int) const = 0;
    virtual std::vector<unsigned int> inRadius(unsigned int, double) const = 0;
    virtual std::vector<unsigned int> neighbours(unsigned int) const = 0;

    Vector getModel(unsigned int n) const {
      Vector v(models+(n*dim), dim);
      return v;
    }
  };

  class RectangularGrid : public Grid {
  public:
    RectangularGrid(unsigned int rows, unsigned int cols, unsigned int dim);
    
    unsigned int index2col(int i) const;
    unsigned int index2row(int i) const;
    unsigned int coords2index(int x, int y) const;
    bool isInsideGrid(int x, int y) const;

    unsigned int distance2(int i, int j) const;

    std::vector<unsigned int> inRadius(unsigned int c, 
				       double r) const;
    std::vector<unsigned int> neighbours(unsigned int i) const;
  };

  class ToroidGrid : public Grid {
    std::vector< std::vector<unsigned int> > calculatedNeighbours;

    std::vector<unsigned int> calculateNeighbours(unsigned int i) const;

  public:
    ToroidGrid(unsigned int rows, unsigned int cols, unsigned int dim);

    unsigned int index2col(int i) const;
    unsigned int index2row(int i) const;
    unsigned int coords2index(int x, int y) const;
    bool isInsideGrid(int x, int y) const;
    
    unsigned int distance2(int i, int j) const;

    std::vector<unsigned int> inRadius(unsigned int c, 
					       double r) const;
    std::vector<unsigned int> neighbours(unsigned int i) const;
  };

  class GridOverlay {
  public:
    virtual double getValue(unsigned int i) = 0;
    virtual std::vector<unsigned int> neighbours(unsigned int) = 0;
    virtual unsigned int size() = 0;
  };

}

#endif
