#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
  int num_count, i;
  FILE* fid;

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
  
  // Open stream as a file
  fid = fopen("/dev/urandom","r");
  if(fid == NULL)
  {
    printf("Failed to open random device\n");
    return -1;
  }
  
  // Read ints
  fread(output,sizeof(int),num_count,fid);
  
  // Close stream
  fclose(fid);
  
  
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
      normalized_output[i] = (((double)output[i]) - INT_MIN) / ((double)INT_MAX - (double)INT_MIN);
    }
  
    // Write numbers to stdout
    for(i=0; i<num_count; i++)
    {
      printf("%lf\n",normalized_output[i]);
    }
  }
  
  
  return 0;
}