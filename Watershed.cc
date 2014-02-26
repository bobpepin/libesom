#include <vector>
#include <set>
#include <queue>
#include <utility>
#include <algorithm>

#include <fstream>
#include <string>

#include "Data.h"
#include "ESOM.h"
#include "Grid.h"
#include "Watershed.h"

namespace esom {

  LabelTree::LabelTree(int threshold) 
  : threshold(threshold), top_label(threshold-1) {}

  int LabelTree::merge(int a, int b) {
    top_label++;
    labels.resize(2*((top_label-threshold)+1));
    labels[2*(top_label-threshold)] = a;
    labels[2*(top_label-threshold)+1] = b;
    return top_label;
  }

  int LabelTree::merge(std::set<int> labels) {
    std::set<int>::const_iterator iter = labels.begin();
    int l = *iter;
    iter++;
    for(; iter != labels.end(); iter++) {
      l = merge(l, *iter);
    }
    return l;
  }

  bool LabelTree::isLeaf(int n) const {
    return n < threshold;
  }
  int LabelTree::leafValue(int n) const {
    return n;
  }
  int LabelTree::left(int n) const {
    assert(!isLeaf(n));
    return labels[2*(n-threshold)];
  }
  int LabelTree::right(int n) const {
    assert(!isLeaf(n));
    return labels[2*(n-threshold)+1];
  }
  int LabelTree::top() const {
    return top_label;
  }

  int LabelTree::leafCount() const {
    return threshold;
  }


  class Pixel_Cmp {
    GridOverlay &grid;
  public:
    Pixel_Cmp(GridOverlay& grid) : grid(grid) {}
    bool operator() (const int& a, const int& b) const {
      return (grid.getValue(a) > grid.getValue(b));
    }
  };

#if 1
    void writePixels(int* pixels, unsigned int n, char* fname) {
      std::ofstream ofs(fname);
      for(unsigned int i=0; i < n; i++) {
        ofs << pixels[i] << '\t';
      }
    }
#endif

  Watershed::Watershed(GridOverlay& grid) : grid(grid) {}

  LabelTree Watershed::tree(std::vector<int>& markers) {
    const int UNPROCESSED = -1;
    const int PROCESSED = -2;
    // Meyer's Watershed algorithm
      
    LabelTree tree(markers.size());
    int *pixel_labels = new int[grid.size()];
    std::fill(pixel_labels, pixel_labels+grid.size(), UNPROCESSED);
    const Pixel_Cmp cmp(grid);
    std::priority_queue<unsigned int, std::vector<unsigned int>, 
			Pixel_Cmp> pixels(cmp);
    std::vector<unsigned int> neighbours;
    
    for(unsigned int i=0; i < markers.size(); i++) {
      unsigned int p = markers[i];
      if(pixel_labels[p] == UNPROCESSED) {
	    pixel_labels[p] = i;
      } else {
	    pixel_labels[p] = tree.merge(pixel_labels[p], i);
      }
    }
    for(unsigned int i=0; i < markers.size(); i++) {
      unsigned int p = markers[i];
      neighbours = grid.neighbours(p);
      for(std::vector<unsigned int>::const_iterator iter = neighbours.begin();
	  iter != neighbours.end(); iter++) {
        if(pixel_labels[*iter] == UNPROCESSED) {
          pixels.push(*iter);
          pixel_labels[*iter] = PROCESSED;
	    }
      }
    }

    writePixels(pixel_labels, grid.size(), "wspixels0.csv");
      
    while(!pixels.empty()) {
      int pixel = pixels.top();
      pixels.pop();
      neighbours = grid.neighbours(pixel);
      std::set<int> labels;
      for(std::vector<unsigned int>::const_iterator p = neighbours.begin(); 
	      p != neighbours.end(); p++) {
	    if(pixel_labels[*p] == UNPROCESSED) {
	      pixels.push(*p);
	      pixel_labels[*p] = PROCESSED;
	    } else if(pixel_labels[*p] != PROCESSED) {
	      labels.insert(pixel_labels[*p]);
	    }
      }
      // If a point is between two differently labeled regions, merge 
      // the two regions and replace their label by a new label issued from
      // the merge. Record the operation in a tree structure.
      assert(labels.size() != 0);
      int assigned_label;
      if(labels.size() == 1) {
	    assigned_label = *(labels.begin());
      } else {
	    assigned_label = tree.merge(labels);
	    for(std::set<int>::const_iterator iter = labels.begin();
          iter != labels.end(); iter++) {
	      std::replace(pixel_labels, pixel_labels+grid.size(), 
                       *iter, assigned_label);
	    }
        char fname[50];
        sprintf(fname, "wspixels%d.csv", assigned_label);
        writePixels(pixel_labels, grid.size(), fname);
      }
      pixel_labels[pixel] = assigned_label;
    }
    delete[] pixel_labels;
    return tree;
  }
}
