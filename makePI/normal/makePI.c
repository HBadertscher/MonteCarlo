/* ********************************************************** *
 * makePI.c                                                   *
 * 24.03.2014 / HBA                                           */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <float.h>

#define NUMOFITERATIONS 10000
 
// Function Prototypes
double getRandom(double min, double max);
int isInCircle(double x, double y);
 
/* ********************************************************** *
 * main                                                       */
int main(int argc, const char * argv[]) {
  int i;
  double x,y;
  int sum;
  double numOfIterations;
  
  if(argc > 1)
    sscanf(argv[1],"%lf",&numOfIterations);
  else
    numOfIterations = NUMOFITERATIONS;
 
  srand(time(NULL));
  
  for(i=0;i<numOfIterations;i++) {
    x = getRandom(0,1);
    y = getRandom(0,1);
//   printf("(%f,%f)\n",x,y);
    if(isInCircle(x,y)) {
      sum++;
    }
  }
  
  //printf("Sum: %d \n", sum);
  
  printf("%f\n", 4*((double)sum)/((double)numOfIterations));
  
  return 0;
 }

/* ********************************************************** *
 * Create a Random Number between min and max                 */
double getRandom(double min, double max)
{
  return (((double)rand())/RAND_MAX)*(max-min)+min;
  
//   FILE* devRandom;
//   unsigned char randomChar;
//   double randomDouble;
//   int i;
//   
//   devRandom = fopen("/dev/urandom", "r");
//   fscanf(devRandom,"%c",&randomInt);
//   fclose(devRandom);
//   
//   return randomDouble;
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