#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
  cl_uint num_platforms;
  int gpu=1;
  int platform = 0;
  int err; 
  int i;
  cl_device_id device_id;             // compute device id
  cl_context context;                 // compute context
  cl_command_queue commands;          // compute command queue
  cl_program program;                 // compute program
  cl_kernel kernel;                   // compute kernel


  // Get Platform Number
  err = clGetPlatformIDs(0, NULL, &num_platforms);
  if( err != CL_SUCCESS)
  {
      printf("Error: Didn't get number of platforms\n");
      return EXIT_FAILURE;
  }
    
  // Allocate memory for platforms
  cl_platform_id *platformIds = (cl_platform_id *)malloc(
      num_platforms * sizeof(cl_platform_id));
  err = clGetPlatformIDs(num_platforms, platformIds, 0);
  if (err != CL_SUCCESS)
  {
      printf("Strange Error: Got number of Platforms but no ID\n");
      return EXIT_FAILURE;
  }
    
  // Get Platform infos
  for(i=0;i<num_platforms;i++)
  {
     size_t size;

      cl_platform_id id = platformIds[i];
      err = clGetPlatformInfo(id,CL_PLATFORM_NAME,0,NULL,&size);
      char* name = (char*)alloca(sizeof(char)*size);
      err = clGetPlatformInfo(id,CL_PLATFORM_NAME,size,name,NULL);
      printf("Platform %d: %s\n",i,name);
  }
  
  // get a compute device of the requested type (GPU/CPU)
	err = clGetDeviceIDs(platformIds[platform],
		gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id,
		NULL);
	if (err != CL_SUCCESS) {
		fprintf(stderr, "%s:%d: no device id found: %d\n",
			__FILE__, __LINE__, err);
		return EXIT_FAILURE;
	}
		fprintf(stderr, "%s:%d: got device id %p\n",
			__FILE__, __LINE__, device_id);
  
	// we want to find out some interesting device parameters, in particular
	// we want to know how large memory, is, cache characteristics and
	// simd width (preferred vector size)
	cl_ulong	globalmemsize = 0, localmemsize = 0, cachesize = 0;
	size_t	paramsize = sizeof(globalmemsize);
	size_t	paramsizeret = 0;
	clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, 
		paramsize, &globalmemsize, &paramsizeret);
	clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, 
		paramsize, &localmemsize, &paramsizeret);
	clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 
		paramsize, &cachesize, &paramsizeret);

	cl_uint	computeunits = 0;
	paramsize = sizeof(computeunits);
	clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS,
		paramsize, &computeunits, &paramsizeret);

	cl_uint cacheline = 0;
	paramsize = sizeof(cacheline);
	clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
		paramsize, &cacheline, &paramsizeret);

	cl_uint	vectorwidth = 0;
	paramsize = sizeof(vectorwidth);
	clGetDeviceInfo(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
		paramsize, &vectorwidth, &paramsizeret);

	cl_bool	unified = 0;
	paramsize = sizeof(unified);
	clGetDeviceInfo(device_id, CL_DEVICE_HOST_UNIFIED_MEMORY,
		paramsize, &unified, &paramsizeret);

    fprintf(stderr, "unified memory:  %s\n",(unified) ? "yes" : "no");
		fprintf(stderr, "global memory:   %lu\n", (unsigned long)globalmemsize);
		fprintf(stderr, "local memory:    %lu\n", (unsigned long)localmemsize);
		fprintf(stderr, "cache size:      %lu\n", (unsigned long)cachesize);
		fprintf(stderr, "cache line size: %u\n", cacheline);
		fprintf(stderr, "compute units:   %u\n", computeunits);
		fprintf(stderr, "vector width:    %u\n", vectorwidth);
    
}