//
//  main.c
//  makePI
//
//  Created by Hannes Badertscher on 29/03/14.
//  Copyright (c) 2014 MathSem2. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <math.h>
#define M_PI 3.14159265358979323846

#include "cluCreateProgramWithFile.c"
#include "park-miller.c"

// main
int main(int argc, const char * argv[])
{
  //---------------------------------------------------------------------
  // Variable Declarations
  int i;                              // Can be used for any for-loop
  unsigned long sim;                  // Number of Simulations

  int debug = 1;                      // Debug Flag
  int platform = 0;                   // Which platform to use

  struct timeval start,stop;          // Used for time measurement  
  float elapsed_time;

  int err = CL_SUCCESS;               // error flag
  
  //---------------------------------------------------------------------
  // Read input arguments
  if(argc < 2) 
  {
    sim = 1024; 
  }
  else
  {
    sim = atoi(argv[1]);
  }
  
  if(debug) fprintf(stderr,"Number of sims:\t %lu\n", sim);
  
  //---------------------------------------------------------------------
  // Get Platforms and plot names
  cl_uint num_platforms;
  err |= clGetPlatformIDs(0, NULL, &num_platforms);
  cl_platform_id *platformIds = (cl_platform_id *)malloc(
                            num_platforms * sizeof(cl_platform_id));
  err |= clGetPlatformIDs(num_platforms, platformIds, 0);
  for(i=0;i<num_platforms;i++)
  {
    size_t size;
    cl_platform_id id = platformIds[i];
    err |= clGetPlatformInfo(id,CL_PLATFORM_NAME,0,NULL,&size);
    char* name = (char*)alloca(sizeof(char)*size);
    err |= clGetPlatformInfo(id,CL_PLATFORM_NAME,size,name,NULL);
    if(debug) fprintf(stderr,"Platform %d: %s\n",i,name);
  }
  
  //---------------------------------------------------------------------
  // Get a compute device and infos about it
  cl_device_id device_id;             // compute device id
  err |= clGetDeviceIDs(platformIds[platform],CL_DEVICE_TYPE_GPU, 1, &device_id,NULL);
  
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
  if(debug == 1) {
    fprintf(stderr, "unified memory:  %s\n",(unified) ? "yes" : "no");
		fprintf(stderr, "global memory:   %lu\n", (unsigned long)globalmemsize);
		fprintf(stderr, "local memory:    %lu\n", (unsigned long)localmemsize);
		fprintf(stderr, "cache size:      %lu\n", (unsigned long)cachesize);
		fprintf(stderr, "cache line size: %u\n", cacheline);
		fprintf(stderr, "compute units:   %u\n", computeunits);
		fprintf(stderr, "vector width:    %u\n", vectorwidth);
  }
 
  //---------------------------------------------------------------------
  // Initialize OpenCL-Stuff
  cl_context	context;
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  
  cl_command_queue	commands;
  commands = clCreateCommandQueue(context, device_id, 0, &err);
  
  if(err != CL_SUCCESS) {
    fprintf(stderr,"Something went wrong in the very beginning: %d\n",err);
    return EXIT_FAILURE;
  }
  
  //---------------------------------------------------------------------
  // Load and build program
  cl_program	program = cluCreateProgramWithFile(context,"makePI.cl", &err);
  	
	err |= clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  	
  if (err) {
	  fprintf(stderr, "%s:%d: cannot compile program: %d\n",__FILE__, __LINE__, err);
		size_t	l = 32 * 1024;
		char	*log = (char *)malloc(l);
		err |= clGetProgramBuildInfo(program, device_id,CL_PROGRAM_BUILD_LOG,l, log, &l);
		if (err) {
			fprintf(stderr, "%s:%d: cannot retrieve log: %d\n",__FILE__, __LINE__, err);
			return EXIT_FAILURE;
		}
		fprintf(stderr, "compile log:\n%*s\n", (int)l, log);
		return EXIT_FAILURE;
	}
	
	//---------------------------------------------------------------------
  // Create Kernel
  cl_kernel	kernel = clCreateKernel(program, "getPoint", &err);
  
  // Get memory requirements
  cl_ulong	localmemreq = 0;
  err = clGetKernelWorkGroupInfo(kernel, device_id,CL_KERNEL_LOCAL_MEM_SIZE, sizeof(localmemreq), &localmemreq,NULL);
  if(debug) fprintf(stderr, "local mem req:   %lu\n", (unsigned long)localmemreq);
  
  // Get Workgroup size
  size_t	workgroupsize;
  err |= clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(workgroupsize), &workgroupsize, NULL);
  if(debug) fprintf(stderr, "work group size: %lu\n", (unsigned long)workgroupsize);

	//---------------------------------------------------------------------
  // Set Kernel Arguments
  
  int* xin = calloc(sizeof(int),sim);
 // int* yin = calloc(sizeof(int),sim);
  if(xin == NULL /*|| yin == NULL*/) {
    fprintf(stderr,"Failed to allocate input buffers.\n");
    return EXIT_FAILURE;
  }
  
  gettimeofday(&start, NULL);
  int seed = time( NULL );
  for(i=0; i<sim; i++) {
    xin[i] = park_miller_rand(&seed);
    park_miller_rand(&seed);
  }
  
  //for(i=0; i<sim; i++) {
    //yin[i] = park_miller_rand(&seed);
  //}
  gettimeofday(&stop, NULL);
  elapsed_time = (float)(stop.tv_usec - start.tv_usec) / 1.0e6 + (stop.tv_sec - start.tv_sec);
  fprintf(stderr,"Elapsed Time for Random: %f  sec\n", elapsed_time);
  
  cl_mem globx = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int)*sim, NULL, NULL);
  //cl_mem globy = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int)*sim, NULL, NULL);
  
  err |= clEnqueueWriteBuffer( commands, globx, CL_TRUE, 0, sizeof(int)*sim, xin, 0, NULL, NULL);
  //err |= clEnqueueWriteBuffer( commands, globy, CL_TRUE, 0, sizeof(int)*sim, yin, 0, NULL, NULL);
  
  err |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &globx);
 // err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &globy);
  
  if(err) {
    fprintf(stderr,"Something's wrong with the Kernel Arguments: %d\n", err);
    return EXIT_FAILURE;
  }
  
  free(xin);
  //free(yin);
	//---------------------------------------------------------------------
  // Execute Kernel
  gettimeofday(&start,NULL);
  size_t local = workgroupsize;
  size_t global = workgroupsize * ((sim / workgroupsize) + ((sim % workgroupsize) ? 1 : 0));
  if(debug) fprintf(stderr,"global size:\t%d\n",(unsigned int)global);
  
  err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
  clFinish(commands);
  gettimeofday(&stop,NULL); 
  elapsed_time = (float)(stop.tv_usec - start.tv_usec) / 1.0e6 + (stop.tv_sec - start.tv_sec);
  fprintf(stderr,"Elapsed Time for OpenCL: %f sec\n", elapsed_time);
  //---------------------------------------------------------------------
  // Read and analyse results
  int* result = calloc(sizeof(int),sim);
  if(result == NULL) {
    fprintf(stderr,"Failed to allocate result buffer.\n");
    return EXIT_FAILURE;
  }
  err |= clEnqueueReadBuffer( commands, globx, CL_TRUE, 0, sizeof(int)*sim, result, 0, NULL, NULL );
  
  unsigned long sum = 0;
  for(i=0; i<sim; i++)
  {
    if(result[i] == 1)
      sum ++;
  }
  double est_pi = 4.0 * (double)sum / (double)sim;
  fprintf(stderr, "Estimated PI: %lf\n", est_pi);
  fprintf(stderr, "Abweichung: %lf\n",fabs(M_PI - est_pi));  
  
  //---------------------------------------------------------------------
  // Clean up
  free(result);
  
  clReleaseMemObject(globx);
  // clReleaseMemObject(globy);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
  
  return EXIT_SUCCESS;
}

