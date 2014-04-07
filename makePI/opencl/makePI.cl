
// BBS PRNG
//#pragma OPENCL EXTENSION cl_khr_fp64 : enable


// I'm probably going to get an angry mob of statisticians with flaming torches and pitchforks showing up any time now!
float rand_int(__global float* seed)
{
    __private float xi = *seed;
    float res;
    float m = (float)9223372036854775907 * (float)9223372036854775931;
    
    xi = fmod(xi*xi,m);
    
    *seed = xi;
    res = fmod(xi,100000);
    res = res / 100000;
    return res;
}


/*
 * Park-Miller random number generator
 */
__constant float a = 16807;
__constant float m = 2147483647;
__constant float reciprocal_m = 1.0 / 2147473647;

int	random(int seed) {
	__private float	temp = seed * a;
	return (int)(temp - m * floor(temp * reciprocal_m));
}



__kernel void getPoint( __global float* output, unsigned int numOfIt, int seed )
{
    __private float x,y;
    __private unsigned int i, sum;
    __private int k = get_global_id(0);
    __private int mySeed;
    
    mySeed = k * seed;

    sum = 0;
    for(i=0; i<numOfIt; i++)
    {
        mySeed = random(mySeed);
        x = (float)mySeed;
        mySeed = random(mySeed);
        y = (float)mySeed;

        x = fmod(x,10000) / 10000;
        y = fmod(y,10000) / 10000;
        
        if( (x*x + y*y) <= 1)
        {
            sum++;
        }
    }
    
    output[k] = ((float)sum) / numOfIt;
}
