#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int main(int argc, char *argv[])
{
  int num_count, i;

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
  int* output = calloc(num_count, sizeof(int));
  if(output == NULL)
  {
    printf("Couldn't allocate output array.\n");
    return -1;
  }
  
  // Seed pRNG
  srand( time(NULL) );
  
  for(i=0; i<num_count; i++)
  {
    output[i] = rand();
  }
  
  // Normalize numbers
  double* normalized_output = calloc(num_count, sizeof(double));
  if(normalized_output == NULL)
  {
    printf("Couldn't allocate normalized output array. Returning not normalized data. \n");
    // Write numbers to stdout
    for(i=0; i<num_count; i++)
    {
      printf("%lf\n",normalized_output[i]);
    }
  }
  else
  {
    for(i=0; i<num_count; i++)
    {
      normalized_output[i] = (((double)output[i])) / ((double)INT_MAX - (double)INT_MIN);
    }
  
    // Write numbers to stdout
    for(i=0; i<num_count; i++)
    {
      printf("%lf\n",normalized_output[i]);
    }
  }
  
  
  return 0;
}