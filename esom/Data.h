#ifndef LIBESOM_DATA_H
#define LIBESOM_DATA_H

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cassert>
#include <iterator>
#include <vector>
#include <map>
#include <algorithm>

namespace esom {

  class Vector {
  protected:
    double *p;
    const unsigned int _size;
  public:
    typedef double value_type;

    typedef double* iterator;
    typedef const double* const_iterator;
    
  Vector(double *p, const unsigned int size) : p(p), _size(size) {};
    
    operator double* () const { return p; }
    operator const double* () const {  return p; }
    
    double& operator[] (int i) { return p[i]; }
    const double& operator[] (int i) const { return p[i]; }

    iterator begin() { return p; }
    const_iterator begin() const { return p; }

    iterator end() { return p+size(); }
    const_iterator end() const { return p+size(); }

    unsigned int size() const { return _size; }

    void operator= (double k) {
      for(iterator i = begin(); i != end(); i++) {
	*i = k;
      }
    }      
    
    void operator= (Vector v) {
      for(unsigned int i=0; i < v.size(); i++) {
	p[i] = v[i];
      }
    }

    void operator*= (double k) {
      for(iterator i = begin(); i != end(); i++) {
	*i *= k;
      }
    }

    void operator/= (double k) {
      for(iterator i = begin(); i != end(); i++) {
	*i /= k;
      }
    }

    void operator+= (Vector v) {
      assert(size() == v.size());
      iterator ai, bi;
      for(ai = begin(), bi = v.begin(); ai != end(); ai++, bi++) {
	*ai += *bi;
      }
    }

    void operator-= (Vector v) {
      assert(size() == v.size());
      iterator ai, bi;
      for(ai = begin(), bi = v.begin(); ai != end(); ai++, bi++) {
	*ai -= *bi;
      }
    }

    void operator-= (double v) {
      iterator ai;
      for(ai = begin(); ai != end(); ai++) {
	*ai -= v;
      }
    }

    void operator*= (Vector v) {
      assert(size() == v.size());
      iterator ai, bi;
      for(ai = begin(), bi = v.begin(); ai != end(); ai++, bi++) {
	*ai *= *bi;
      }
    }

    void operator/= (Vector v) {
      assert(size() == v.size());
      iterator ai, bi;
      for(ai = begin(), bi = v.begin(); ai != end(); ai++, bi++) {
	*ai /= *bi;
      }
    }

    void pow(int n) {
      for(iterator i = begin(); i != end(); i++) {
	*i = std::pow(*i, n);
      }
    }

    void pow(double n) {
      for(iterator i = begin(); i != end(); i++) {
	*i = std::pow(*i, n);
      }
    }

  };
  
  class MemVector : public Vector {
  private:
    MemVector(const MemVector&);
    MemVector& operator=(const MemVector&);
  public:
  MemVector(unsigned int n) : Vector(new double[n], n) { }

    ~MemVector() {
      delete[] p;
    }
  };

  class Matrix {

    double *p;
    int _rows, _cols;

  public:

  Matrix(double *p, int rows, int cols) 
    : p(p), _rows(rows), _cols(cols) {};

    operator double* () { return p; }

    const double& operator() (int i, int j) const {
      return p[i*_cols + j];
    }

    double& operator() (int i, int j) {
      return p[i*_cols + j];
    }

    const Vector operator() (int i) const {
      return Vector(&p[i*_cols], _cols);
    }

    Vector operator() (int i) {
      return Vector(&p[i*_cols], _cols);
    }

    double* data() const {
      return p;
    }
    
    unsigned int rows() const {
      return _rows;
    }
    unsigned int columns() const {
      return _cols;
    }
  };
}


#endif
