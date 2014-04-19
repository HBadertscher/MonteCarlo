#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define N     624
#define M     397
#define HI    0x80000000
#define LO    0x7fffffff


double mersenne_twister(uint32_t* y) 
{
  static const uint32_t A[2] = { 0, 0x9908b0df };
  static int index = N;
  static const uint32_t seed = 5489UL;
  uint32_t  e;
 
  // Berechne neuen Zustandsvektor
  if (index >= N) 
  {
    int i;
    uint32_t h;
 
    for(i = 0; i < N-M; i++) 
    {
      h = (y[i] & HI) | (y[i+1] & LO);
      y[i] = y[i+M] ^ (h >> 1) ^ A[h & 1];
    }
    for( ; i < N-1; i++)
    {
      h = (y[i] & HI) | (y[i+1] & LO);
      y[i] = y[i+(M-N)] ^ (h >> 1) ^ A[h & 1];
    }
 
    h = (y[N-1] & HI) | (y[0] & LO);
    y[N-1] = y[M-1] ^ (h >> 1) ^ A[h & 1];
    index = 0;
  }
 
  e = y[index++];
  
  // Gleichverteilung sicherstellen
  e ^= (e >> 11);
  e ^= (e <<  7) & 0x9d2c5680;
  e ^= (e << 15) & 0xefc60000;
  e ^= (e >> 18);
 
  return (double)e / (double)UINT_MAX;
}

int main(int argc, char *argv[])
{
  int num_count, i;
  uint32_t* y;
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
  
  // Allocate memory
  double* output = calloc(num_count, sizeof(double));
  if(output == NULL)
  {
    printf("Couldn't allocate output array.\n");
    return -1;
  }
  y = calloc(N,sizeof(uint32_t));
  if(y == NULL)
  {
    printf("Couldn't allocate states.\n");
    return -1;
  }
  
  // Seed Mersenne Twister with 624 bytes from /dev/urandom
  fid = fopen("/dev/urandom","r");
  if(fid == NULL)
  {
    printf("Failed to open random device\n");
    return -1;
  }
  fread(y,sizeof(uint32_t),624,fid);
  fclose(fid);
  
  // Create num_count numbers
  for(i=0; i<num_count; i++)
  {
    output[i] = mersenne_twister(y);
    printf("%lf\n", output[i]);
  }

  return 0;
}