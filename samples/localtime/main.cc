#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{

printf("enter\n");
  struct tm tmp_tm;
  time_t tmp_t;
  
  localtime_r(&tmp_t, &tmp_tm);
printf("leave\n");
  return 0;
}
