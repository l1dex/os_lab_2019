#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;

  unsigned int i = begin;
  min_max.max = *(array+i);
  min_max.min = *(array+i);
  while(i!=end){
      if(min_max.min > *(array+i)) min_max.min = *(array+i);
      if(min_max.max < *(array+i)) min_max.max = *(array+i);
      ++i;
  }
  
  return min_max;
}
