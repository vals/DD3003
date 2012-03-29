//
// Hello World for OpenCL
// Copyright 2009 David Black-Schaffer
//

#include "opencl_utils.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>

#define LENGTH 1024*1024
#define DATA_SIZE sizeof(cl_float)*LENGTH



// Note: there is no error handling here. This is bad practice in general.

int main (int argc, const char * argv[]) {
	
	// OpenCL Objects
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem buffer;	
	
	
	// Create and initialize the input data
	cl_float *data;
	data = (cl_float*)malloc(DATA_SIZE);
	for (int i=0; i<LENGTH; i++) 
		data[i] = i;
	
	// Get the platform
	// Note: I cheated in the slides. You have to get the platform before getting the device.
	cl_platform_id platform;
	clGetPlatformIDs(1, &platform, NULL);
	
	// Setup OpenCL
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	queue = clCreateCommandQueue(context, device, (cl_command_queue_properties)0, NULL);	
	
	// Define our kernel. It just calculates the sin of the input data.
	char *source = {
		"kernel void calcSin(global float *data) {\n"
		"  int id = get_global_id(0);\n"
		"  data[id] = cos(data[id]);\n"
		"}\n"
	};	
	
	// Compile the kernel
	program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "calcSin", NULL);
	
	// Create the memory object
	buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE, NULL, NULL);
	
	// Copy the data to the input
	clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, data, 0, NULL, NULL);
	
	// Execute the kernel
	clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
	size_t global_dimensions[] = {LENGTH,0,0};
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL);
	
	// Read back the results
	clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(cl_float)*LENGTH, data, 0, NULL, NULL);
	
	// Wait for everything to finish
	clFinish(queue);
	
	// Clean up
	clReleaseMemObject(buffer);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
	// Print out the results
	//for (int i=0; i<LENGTH; i++)
	//	printf("cos(%d) = %f\n", i, data[i]);
	
	free(data);
}
