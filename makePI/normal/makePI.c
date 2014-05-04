/* ********************************************************** *
 * makePI.c                                                   *
 * 24.03.2014 / HBA                                           */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <float.h>

#include "park-miller.c"
#define NUMOFITERATIONS 10000
 
// Function Prototypes
int isInCircle(double x, double y);
 
/* ********************************************************** *
 * main                                                       */
int main(int argc, const char * argv[]) {
  int i;
  double x,y;
  int sum;
  double numOfIterations;
  struct timeval start, stop;  
  float elapsedTime;

  if(argc > 1)
    sscanf(argv[1],"%lf",&numOfIterations);
  else
    numOfIterations = NUMOFITERATIONS;
 
  int seed = (time(NULL));
  gettimeofday(&start,NULL);  

  for(i=0;i<numOfIterations;i++) {
    x = park_miller_rand(&seed);
    y = park_miller_rand(&seed);
//   printf("(%f,%f)\n",x,y);
    if(isInCircle(x,y)) {
      sum++;
    }
  }
  gettimeofday(&stop,NULL);
  //printf("Sum: %d \n", sum);
  
  printf("Result: %f\n", 4*((double)sum)/((double)numOfIterations));
  elapsedTime = (float)(stop.tv_usec - start.tv_usec) / 1.0e6 + (stop.tv_sec - start.tv_sec); 
  printf("Elapsed Time: %f\n",elapsedTime);
  return 0;
 }


/* ********************************************************** *
 * Checks if a point (x,y) is inside the unit circle          */
int isInCircle(double x, double y)
{
  double res;
  
  res = x*x + y*y;
  
  if(res <= 1)
    return 1;
  else
    return 0;
}
