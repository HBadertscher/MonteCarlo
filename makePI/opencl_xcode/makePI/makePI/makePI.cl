#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable

// I'm probably going to get an angry mob of statisticians with flaming torches and pitchforks showing up any time now!
int rand_int(int* seed)
{
    unsigned int xi = *(unsigned int*)seed;
    unsigned int m = 65537 * 67777;
    
    xi = (xi * xi) % m;
    *seed = xi;
    return xi % 1000 ;
}

__kernel void getPoint( __global ulong* output )
{
    __private float x,y;
    __private uint k;
    __private int i = get_global_id(0);
    __private uint sum;
    
   x = (float)rand_int(&i) / 1000;
   y = (float)rand_int(&i) / 1000;
        
   if( (x*x + y*y) <= 1)
       atom_inc(output);
    
}
