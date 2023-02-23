#include "block.h"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int Block::height() const {
  /* your code here */
  if (!data.empty()) {
    return data[0].size();
  } else {
    return 0;
  }
  
}

int Block::width() const {
  /* your code here */
  return data.size();
}

void Block::render(PNG &im, int x) const {
  /* your code here */

  for (unsigned int i = 0; i <= data.size()-1; i++) {
    for (unsigned int j = 0; j <= data[i].size()-1; j++) {

      HSLAPixel px = data[i].at(j);
      *im.getPixel(i + x, j) = px;
    }

  }


}

void Block::build(PNG &im, int x, int width) {

  int height = im.height();
  /* your code here */
  for (int i = x; i <= x+width-1; i++) {
    vector<HSLAPixel> block;
    for (int j = 0; j <= height-1; j++) {
      block.push_back(*im.getPixel(i, j));
    }
    data.push_back(block);
  }
}
