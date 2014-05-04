//#pragma OPENCL EXTENSION cl_khr_fp64 : enable



int park_miller_rand(int* seed)
{
  const int a = 16807;
  const int m = 2147483647;
  const int q = 127773;
  const int r = 2836;

  int k=*seed/q;
  *seed = a * (*seed - k*q) - k*r;
  if(*seed < 0) *seed += m;
  return *seed;
}

__kernel void getPoint( __global unsigned int* globx )
{
  const int m = 2147483647;
  __private int k = get_global_id(0);
  __private int seed = globx[k];  
  
  float x = (float)seed / m;
  float y = (float)park_miller_rand(&seed) / m;
    
  if( (x*x + y*y) <= 1)
  {
    globx[k] = 1;
  }
  else
  {
    globx[k] = 0;
  }    
  return;
}
