#ifndef LIBESOM_WATERSHED_H
#define LIBESOM_WATERSHED_H

#include <vector>
#include <set>
#include <queue>
#include <utility>
#include <algorithm>

#include <fstream>
#include <string>

namespace esom {

  class LabelTree {
    int threshold;
    int top_label;
    std::vector<int> labels;
  public:

    // threshold is the label of the first non-leaf node
    LabelTree(int threshold = 0);

    int merge(int a, int b);
    int merge(std::set<int> labels);

    bool isLeaf(int n) const;
    int leafValue(int n) const;
    int left(int n) const;
    int right(int n) const;
    int top() const;
    int leafCount() const;
  };

  class Watershed {
    GridOverlay& grid;

  public:
    Watershed(GridOverlay& grid);
    LabelTree tree(std::vector<int>& markers);
  };
}

#endif
