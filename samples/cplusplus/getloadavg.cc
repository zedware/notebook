/*
g++ -Wall getloadavg.cc
 */
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main(void)
{
  int rc = 0;
  double loadavg[5];
  
  rc = getloadavg(loadavg, 5);
  if (rc != -1) {
    for (int i = 0; i < rc; i++) {
      printf("%f\n", loadavg[i]);
    }
  }
  
  return 0;
}
