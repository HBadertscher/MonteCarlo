#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


double park_miller_rand(int* seed)
{
  const int a = 16807;
  const int m = 2147483647;
  const int q = 127773;
  const int r = 2836;

  int k=*seed/q;
  *seed = a * (*seed - k*q) - k*r;
  if(*seed < 0) *seed += m;
  return (double)*seed / m;
}

int main(int argc, char *argv[])
{
  int num_count, i;
  int seed;

  // Read input arguments
  if(argc < 2) 
  {
    num_count = 10; // Default: generate 10 numbers
  }
  else
  {
    num_count = atoi(argv[1]);
  }
  
  // Generate random numbers
  double* output = calloc(num_count, sizeof(double));
  if(output == NULL)
  {
    printf("Couldn't allocate output array.\n");
    return -1;
  }
  
  // Seed pRNG
  seed = (int)time(NULL);  
  for(i=0; i<num_count; i++)
  {
    output[i] = park_miller_rand(&seed);
  }
  
  // Write numbers to stdout
  for(i=0; i<num_count; i++)
  {
    printf("%lf\n",output[i]);
  }
    
  return 0;
}