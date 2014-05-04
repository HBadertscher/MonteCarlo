#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <fcntl.h>
#include <alloca.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

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
