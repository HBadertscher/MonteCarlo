//#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void pmRandom( int seed, __global int* rand)
{
  
}

__kernel void getPoint( __global unsigned int* globx, __global unsigned int* globy )
{
  const int m = 2147483647;
  __private int k = get_global_id(0);
    
  float x = (float)globx[k] / m;
  float y = (float)globy[k] / m;
    
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
