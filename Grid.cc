#include "Data.h"
#include "ESOM.h"
#include "Grid.h"

namespace esom {

  Grid::Grid(unsigned int rows, unsigned int columns, unsigned int dim)
    : _rows(rows), _columns(columns), dim(dim)
  {
    models = new double[rows*columns*dim];
  };

  Grid::~Grid() {
    delete[] models;
  }


  RectangularGrid::RectangularGrid(unsigned int rows, 
				   unsigned int cols, unsigned int dim)
    : Grid(rows, cols, dim) {}

  unsigned int RectangularGrid::index2col(int i) const {
    return (i % columns());
  }
  unsigned int RectangularGrid::index2row(int i) const {
    return (i / columns());
  }
  unsigned int RectangularGrid::coords2index(int x, int y) const {
    return y*columns() + x;
  }
  bool RectangularGrid::isInsideGrid(int x, int y) const {
    return (x >= 0 && x < (int)columns() && y >= 0 && y < (int)rows());
  }    

  unsigned int RectangularGrid::distance2(int i, int j) const {
    int xi = index2col(i);
    int yi = index2row(i);
    int xj = index2col(j);
    int yj = index2row(j);
    int dx = xi - xj;
    int dy = yi - yj;

    return (dx*dx)+(dy*dy);
  }

  std::vector<unsigned int> RectangularGrid::inRadius(unsigned int c, 
							      double r) const {
    std::vector<unsigned int> n;
    int rr = std::ceil(r);
    int xc = index2col(c);
    int yc = index2row(c);
    for(int x=xc-rr; x <= xc+rr; x++) {
      for(int y=yc-rr; y <= yc+rr; y++) {
	if(isInsideGrid(x, y)) {
	  n.push_back(coords2index(x, y));
	}
      }
    }
    return n;
  }

  std::vector<unsigned int> RectangularGrid::neighbours(unsigned int i) const {
    std::vector<unsigned int> n;
    n.reserve(4);
    unsigned int x = index2col(i);
    unsigned int y = index2row(i);
    if(x > 0)
      n.push_back(coords2index(x-1, y));
    if(x < columns()-1)
      n.push_back(coords2index(x+1, y));
    if(y > 0)
      n.push_back(coords2index(x, y-1));
    if(y < rows()-1)
      n.push_back(coords2index(x, y+1));
    return n;
  }


  ToroidGrid::ToroidGrid(unsigned int rows, unsigned int cols, 
			 unsigned int dim) 
    : Grid(rows, cols, dim), calculatedNeighbours(rows*cols)
  {
    for(unsigned int i=0; i < rows*cols; i++) {
      calculatedNeighbours[i] = calculateNeighbours(i);
    }
  }

  unsigned int ToroidGrid::index2col(int i) const {
    return (i % columns());
  }
  unsigned int ToroidGrid::index2row(int i) const {
    return (i / columns());
  }
  unsigned int ToroidGrid::coords2index(int x, int y) const {
    return ((y+rows())%rows())*columns() + ((x+columns())%columns());
  }
  bool ToroidGrid::isInsideGrid(int x, int y) const {
    return true;
  }

  unsigned int ToroidGrid::distance2(int i, int j) const {
    using std::min;
    using std::max;
    using std::abs;
    int xi = index2col(i);
    int yi = index2row(i);
    int xj = index2col(j);
    int yj = index2row(j);
    int x1 = min(xi, xj);
    int x2 = max(xi, xj);
    int y1 = min(yi, yj);
    int y2 = max(yi, yj);
    int dx = min(abs(x1 - x2), abs(x1 + (int)columns() - x2));
    int dy = min(abs(y1 - y2), abs(y1 + (int)rows() - y2));
      
    return (dx*dx)+(dy*dy);
  }

  std::vector<unsigned int> ToroidGrid::inRadius(unsigned int c, 
							 double r) const {
    std::vector<unsigned int> n;
    int rr = std::ceil(r);
    int xc = index2col(c);
    int yc = index2row(c);
    for(int x=xc-rr; x <= xc+rr; x++) {
      for(int y=yc-rr; y <= yc+rr; y++) {
	n.push_back(coords2index(x, y));
      }
    }
    return n;
  }

  std::vector<unsigned int> ToroidGrid::calculateNeighbours(unsigned int i) const {
    std::vector<unsigned int> n(4);
    int x = index2col(i);
    int y = index2row(i);
    n[0] = coords2index(x-1, y);
    n[1] = coords2index(x+1, y);
    n[2] = coords2index(x, y-1);
    n[3] = coords2index(x, y+1);
    return n;
  }

  std::vector<unsigned int> ToroidGrid::neighbours(unsigned int i) const {
    return calculatedNeighbours[i];
  }

}
