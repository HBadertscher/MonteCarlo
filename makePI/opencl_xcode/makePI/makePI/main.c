//
//  main.c
//  makePI
//
//  Created by Hannes Badertscher on 29/03/14.
//  Copyright (c) 2014 MathSem2. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <OpenCL/opencl.h>
#include <limits.h>


// Number of Iterations
#define NUM_OF_IT 10000


/*
 * \brief Auxiliary function to read OpenCL code from a file
 *
 * This function reads the contents of a file and invokes the
 * clCreateProgramWithSource to create a program.
 */
cl_program	cluCreateProgramWithFile(cl_context context,
                                     const char *filename, cl_int *err) {
	// first check that the file exists, but using stat will also give
	// the file size which can later be used to size a buffer
	struct stat	sb;
	if (stat(filename, &sb) < 0) {
		*err = -1;
		return NULL;
	}
    
	// open the file for reading
	int	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		*err = -1;
		return NULL;
	}
    
	// read the file data into a memory buffer, allocated on the stack
	// from the file size info in the stat structure received previously
	size_t	length[1];
	length[0] = sb.st_size;
	char	*source[1];
	source[0] = alloca(sb.st_size);
	if (sb.st_size != read(fd, source[0], sb.st_size)) {
		*err = -1;
		return NULL;
	}
	close(fd);
    
	// use the clCreateProgramWithSource function to create the program
	cl_program	program;
	program = clCreateProgramWithSource(context, 1, (const char **)source,
                                        length, err);

	return program;
}


// main
int main(int argc, const char * argv[])
{
    uint numOfCalc = 1000;
    uint count = 1024*NUM_OF_IT / numOfCalc;
    
    float result[count];                   // results returned from device
    float pi;
    int err;                            // error code returned from api calls
    int i;

    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    
    cl_device_id device_id;             // compute device id
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    
    cl_mem output;                      // device memory used for the output array
    
    
    // Connect to a compute device
    //
    int gpu = 1;
    err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }

    // Create a compute context
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }
    
    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }
    
    //program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    program = cluCreateProgramWithFile(context,"/Users/boss/Documents/Studium/HBadertscher/MonteCarlo/makePI/opencl_xcode/makePI/makePI/makePI.cl", &err);
    if (!program) {
        printf("Error: Failed to create compute program!\n");
		return EXIT_FAILURE;
	}

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
        
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
    
    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "getPoint", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
    
    // Create the input and output arrays in device memory for our calculation
    //
    output = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float)*count, NULL, NULL);
    if (!output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }
    
    // Set the arguments to our compute kernel
    //
    err = 0;
    err |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel using the maximum number of work group items for this device
    //
    global = count;//local*((int)count/local);
    if(result == 0)
    {
        printf("Error: Failed to allocate memory");
        return EXIT_FAILURE;
    }
    
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel! %d\n",err);
        return EXIT_FAILURE;
    }
    
    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float)*count, &result, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    
    // Validate our results
    //
    
    pi = 0;
    for(i=0; i<count; i++)
    {
        pi += result[i];
    }
    pi = 4 * pi / count;
    printf("%f\n", pi);

    
    // release all the objects
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
    
	return EXIT_SUCCESS;
}
