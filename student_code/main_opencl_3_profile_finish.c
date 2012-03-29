#include "parameters.h"
#include "util.h"
#include "opencl_utils.h"


// Performance measurement
perf program_perf, create_perf, write_perf, read_perf, finish_perf, cleanup_perf;
perf total_perf, update_perf, range_perf;
void init_all_perfs();
void print_perfs();

// OpenCL
void update_cl(float *in, float *out,cl_program program,cl_kernel kernel);
cl_device_id opencl_device;
cl_context opencl_context;
cl_command_queue opencl_queue;

int main (int argc, const char * argv[]) {
	printf("2. OpenCL Profile\n");

	float range = BIG_RANGE;
	float *in, *out;
	int iterations = 0;
	
	// ======== Initialize
	init_all_perfs();
	create_data(&in, &out);
	start_perf_measurement(&total_perf);

	// ======== Setup OpenCL
	setup_cl(argc, argv, &opencl_device, &opencl_context, &opencl_queue);
	
	cl_int error;

        // Load the program source
        char* program_text = load_source_file("kernel.cl");
        if (program_text == NULL) {
	  printf("Failed to load source file.\n");
	  exit(-1);
        }

        // Create the program

        cl_program program;
        start_perf_measurement(&program_perf);
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
        stop_perf_measurement(&program_perf);

	// ======== Compute
	while (range > LIMIT) {

		// Calculation
	  update_cl(in, out,program,kernel);
		
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

	clReleaseKernel(kernel);
        clReleaseProgram(program);
        free(program_text);
	free(in);
	free(out);
}



void update_cl(float *in, float *out,cl_program program,cl_kernel kernel) {
	cl_int error;
	
	/*// Load the program source
	char* program_text = load_source_file("kernel.cl");
	if (program_text == NULL) {
		printf("Failed to load source file.\n");
		exit(-1);
	}
	
	// Create the program
	
	cl_program program;
	start_perf_measurement(&program_perf);
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
	stop_perf_measurement(&program_perf);*/


	// Create the data objects
	cl_mem in_buffer, out_buffer;
	start_perf_measurement(&create_perf);
	in_buffer = clCreateBuffer(opencl_context, CL_MEM_READ_ONLY, SIZE_BYTES, NULL, &error);
	clFinish(opencl_queue);
	stop_perf_measurement(&create_perf);

	checkError(error, "clCreateBuffer");
	out_buffer = clCreateBuffer(opencl_context, CL_MEM_WRITE_ONLY, SIZE_BYTES, NULL, &error);
	checkError(error, "clCreateBuffer");
	
	// Copy data to the device
	start_perf_measurement(&write_perf);
	error = clEnqueueWriteBuffer(opencl_queue, in_buffer, CL_FALSE, 0, SIZE_BYTES, in, 0, NULL, NULL);
	clFinish(opencl_queue);
	stop_perf_measurement(&write_perf);

	checkError(error, "clEnqueueWriteBuffer");

	start_perf_measurement(&write_perf);
	error = clEnqueueWriteBuffer(opencl_queue, out_buffer, CL_FALSE, 0, SIZE_BYTES, out, 0, NULL, NULL);
	clFinish(opencl_queue);
	stop_perf_measurement(&write_perf);

	checkError(error, "clEnqueueWriteBuffer");
	
	// Set the kernel arguments
	start_perf_measurement(&update_perf);
	error = clSetKernelArg(kernel, 0, sizeof(in_buffer), &in_buffer);
	checkError(error, "clSetKernelArg in");
	error = clSetKernelArg(kernel, 1, sizeof(out_buffer), &out_buffer);
	checkError(error, "clSetKernelArg out");
	stop_perf_measurement(&update_perf);
	
	// Enqueue the kernel
	size_t global_dimensions[] = {SIZE,SIZE,0}; // Ignore the border

	start_perf_measurement(&update_perf);
	error = clEnqueueNDRangeKernel(opencl_queue, kernel, 2, NULL, global_dimensions, NULL, 0, NULL, NULL);
	clFinish(opencl_queue);
	stop_perf_measurement(&update_perf);

	checkError(error, "clEnqueueNDRangeKernel");
	
	// Enqueue a read to get the data back
	start_perf_measurement(&read_perf);
	error = clEnqueueReadBuffer(opencl_queue, out_buffer, CL_FALSE, 0, SIZE_BYTES, out, 0, NULL, NULL);
	clFinish(opencl_queue);
	stop_perf_measurement(&read_perf);

	checkError(error, "clEnqueueReadBuffer");
	
	// Wait for it to finish
	start_perf_measurement(&finish_perf);
	error = clFinish(opencl_queue);
	stop_perf_measurement(&finish_perf);
	
	checkError(error, "clFinish");

	
	// Cleanup
	start_perf_measurement(&cleanup_perf);
	clReleaseMemObject(out_buffer);
	clReleaseMemObject(in_buffer);
	/*clReleaseKernel(kernel);
	clReleaseProgram(program);
	free(program_text);*/
	stop_perf_measurement(&cleanup_perf);

}





void init_all_perfs() {
	init_perf(&total_perf);
	init_perf(&update_perf);
	init_perf(&range_perf);	
	init_perf(&program_perf);
	init_perf(&create_perf);
	init_perf(&write_perf);
	init_perf(&read_perf);
	init_perf(&finish_perf);
	init_perf(&cleanup_perf);
}

void print_perfs() {
	printf("Total:          ");
	print_perf_measurement(&total_perf);
	printf("Update Kernel:  ");
	print_perf_measurement(&update_perf);
	printf("Range Compute:  ");
	print_perf_measurement(&range_perf);
	printf("Read Data:      ");
	print_perf_measurement(&read_perf);
	
	printf("Compile Program:");
	print_perf_measurement(&program_perf);
	printf("Create Buffers: ");
	print_perf_measurement(&create_perf);
	printf("Write Data:     ");
	print_perf_measurement(&write_perf);
	printf("Finish:         ");
	print_perf_measurement(&finish_perf);
	printf("Cleanup:        ");
	print_perf_measurement(&cleanup_perf);
}	
