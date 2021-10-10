#include <stdio.h>

#define Max(x, y)   ((x) > (y) ? (x) : (y))

int main(void) {
  printf("hello world\n");
  printf("Max(%d, %d) = %d", 1, 2, Max(1, 2));
  return 0;
}
