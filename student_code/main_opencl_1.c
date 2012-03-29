#include "parameters.h"
#include "util.h"
#include "opencl_utils.h"


// Compute functions
float find_range(float *data, int size);				// Find the maximum range

// Performance measurement
perf total_perf, update_perf, range_perf;
void init_all_perfs();
void print_perfs();

// OpenCL
void update_cl(float *in, float *out);
cl_device_id opencl_device;
cl_context opencl_context;
cl_command_queue opencl_queue;


int main (int argc, const char * argv[]) {
	printf("1. OpenCL\n");
	
	float range = BIG_RANGE;
	float *in, *out;
	int iterations = 0;
	
	// ======== Initialize
	init_all_perfs();
	create_data(&in, &out);
	start_perf_measurement(&total_perf);
	
	// ======== Setup OpenCL
	setup_cl(argc, argv, &opencl_device, &opencl_context, &opencl_queue);
	
	// ======== Compute
	while (range > LIMIT) {

		// Calculation
		start_perf_measurement(&update_perf);
		update_cl(in, out);
		stop_perf_measurement(&update_perf);
		
		// Compute Range
		start_perf_measurement(&range_perf);
		range = find_range(out, SIZE*SIZE);
		stop_perf_measurement(&range_perf);
		
		iterations++;
		swap(&in, &out);	

		printf("Iteration %d, range=%f.\n", iterations, range);
	}	
	
	stop_perf_measurement(&total_perf);
	print_perfs();
	
	free(in);
	free(out);
}


void update_cl(float *in, float *out) {
	cl_int error;
	
	// Load the program source
	char* program_text = load_source_file("kernel.cl");
	if (program_text == NULL) {
		printf("Failed to load source file.\n");
		exit(-1);
	}
	
	// Create the program
	cl_program program;
	program = clCreateProgramWithSource(opencl_context, 1, (const char**)&program_text, NULL, &error);
	checkError(error, "clCreateProgramWithSource");
	
	// Compile the program and check for errors
	error = clBuildProgram(program, 1, &opencl_device, NULL, NULL, NULL);
	// Get the build errors if there were any
	if (error != CL_SUCCESS) {
		printf("clCreateProgramWithSource failed (%d). Getting program build log.\n", error);
		cl_int error2;
		char build_log[10000];
		error2 = clGetProgramBuildInfo(program, opencl_device, CL_PROGRAM_BUILD_LOG, 10000, build_log, NULL);
		checkError(error2, "clGetProgramBuildInfo");
		printf("Build Failed. Log:\n%s\n", build_log);
	}
	checkError(error, "clBuildProgram");
	
	// Create the computation kernel
	cl_kernel kernel = clCreateKernel(program, "update", &error);
	checkError(error, "clCreateKernel");
	
	// Create the data objects
	cl_mem in_buffer, out_buffer;
	in_buffer = clCreateBuffer(opencl_context, CL_MEM_READ_ONLY, SIZE_BYTES, NULL, &error);
	checkError(error, "clCreateBuffer");
	out_buffer = clCreateBuffer(opencl_context, CL_MEM_WRITE_ONLY, SIZE_BYTES, NULL, &error);
	checkError(error, "clCreateBuffer");
	
	// Copy data to the device
	error = clEnqueueWriteBuffer(opencl_queue, in_buffer, CL_FALSE, 0, SIZE_BYTES, in, 0, NULL, NULL);
	checkError(error, "clEnqueueWriteBuffer");
	error = clEnqueueWriteBuffer(opencl_queue, out_buffer, CL_FALSE, 0, SIZE_BYTES, out, 0, NULL, NULL);
	checkError(error, "clEnqueueWriteBuffer");
	
	// Set the kernel arguments
	error = clSetKernelArg(kernel, 0, sizeof(in_buffer), &in_buffer);
	checkError(error, "clSetKernelArg in");
	error = clSetKernelArg(kernel, 1, sizeof(out_buffer), &out_buffer);
	checkError(error, "clSetKernelArg out");
	
	// Enqueue the kernel
	size_t global_dimensions[] = {SIZE,SIZE,0};
	error = clEnqueueNDRangeKernel(opencl_queue, kernel, 2, NULL, global_dimensions, NULL, 0, NULL, NULL);
	checkError(error, "clEnqueueNDRangeKernel");

	// Enqueue a read to get the data back
	error = clEnqueueReadBuffer(opencl_queue, out_buffer, CL_FALSE, 0, SIZE_BYTES, out, 0, NULL, NULL);
	checkError(error, "clEnqueueReadBuffer");
	
	// Wait for it to finish
	error = clFinish(opencl_queue);
	checkError(error, "clFinish");
	
	// Cleanup
	clReleaseMemObject(out_buffer);
	clReleaseMemObject(in_buffer);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	free(program_text);
}






void init_all_perfs() {
	init_perf(&total_perf);
	init_perf(&update_perf);
	init_perf(&range_perf);
}

void print_perfs() {
	printf("Total:  ");
	print_perf_measurement(&total_perf);
	printf("Update: ");
	print_perf_measurement(&update_perf);
	printf("Range:  ");
	print_perf_measurement(&range_perf);
}	