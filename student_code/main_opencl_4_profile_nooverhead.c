#include "parameters.h"
#include "util.h"
#include "opencl_utils.h"


// Performance measurement
perf program_perf, create_perf, write_perf, read_perf, finish_perf, cleanup_perf;
perf total_perf, update_perf, range_perf;
void init_all_perfs();
void print_perfs();

// OpenCL
cl_mem get_in_buffer();
cl_mem get_out_buffer();
void update_cl(cl_mem in_b, cl_mem out_b);
void setup_cl_compute();
void copy_data_to_device(float *in, float *out);
void read_back_data(cl_mem buffer_to_read_from, float *result_buffer);
void cleanup_cl();
cl_device_id opencl_device;
cl_context opencl_context;
cl_command_queue opencl_queue;
cl_program opencl_program;
cl_kernel update_kernel;
cl_mem in_buffer, out_buffer;

int iterations = 0;

int main (int argc, const char * argv[]) {
	printf("4. OpenCL Profile No Overhead in the Loop\n");

	float range = BIG_RANGE;
	float *in, *out;
		
	// ======== Initialize
	init_all_perfs();
	create_data(&in, &out);
	start_perf_measurement(&total_perf);
	
	// ======== Setup OpenCL
	setup_cl(argc, argv, &opencl_device, &opencl_context, &opencl_queue);
		
	// ======== Setup the computation
	setup_cl_compute();
	start_perf_measurement(&write_perf);
	copy_data_to_device(in, out);
	stop_perf_measurement(&write_perf);	
	
	// ======== Compute
	while (range > LIMIT) {

		// Calculation
		start_perf_measurement(&update_perf);
		update_cl(get_in_buffer(), get_out_buffer());
		stop_perf_measurement(&update_perf);

		// Read back the data
		start_perf_measurement(&read_perf);
		read_back_data(get_out_buffer(), out);
		stop_perf_measurement(&read_perf);
		
		// Compute Range
		start_perf_measurement(&range_perf);
		range = find_range(out, SIZE*SIZE);
		stop_perf_measurement(&range_perf);
		
		iterations++;

		printf("Iteration %d, range=%f.\n", iterations, range);
	}	
	
	// ======== Finish and cleanup OpenCL
	start_perf_measurement(&finish_perf);
	clFinish(opencl_queue);
	stop_perf_measurement(&finish_perf);
		
	start_perf_measurement(&cleanup_perf);
	cleanup_cl();
	stop_perf_measurement(&cleanup_perf);
	
	stop_perf_measurement(&total_perf);
	print_perfs();
	
	free(in);
	free(out);
}

cl_mem get_in_buffer() {
	if (iterations % 2 == 0)
		return in_buffer;
	return out_buffer;
}

cl_mem get_out_buffer() {
	if (iterations % 2 == 0)
		return out_buffer;
	return in_buffer;
}

void update_cl(cl_mem in_b, cl_mem out_b) {
	cl_int error;
	// Set the kernel arguments
        start_perf_measurement(&update_perf);
        error = clSetKernelArg(update_kernel, 0, sizeof(in_b), &in_b);
        checkError(error, "clSetKernelArg in");
        error = clSetKernelArg(update_kernel, 1, sizeof(out_b), &out_b);
        checkError(error, "clSetKernelArg out");
        //stop_perf_measurement(&update_perf);

        // Enqueue the kernel
        size_t global_dimensions[] = {SIZE,SIZE,0}; // Ignore the border

        //start_perf_measurement(&update_perf);
        error = clEnqueueNDRangeKernel(opencl_queue, update_kernel, 2, NULL, global_dimensions, NULL, 0, NULL, NULL);
        clFinish(opencl_queue);
        stop_perf_measurement(&update_perf);

        checkError(error, "clEnqueueNDRangeKernel");
}


void read_back_data(cl_mem buffer_to_read_from, float *result_buffer) {
	cl_int error;
	// Enqueue a read to get the data back
	start_perf_measurement(&read_perf);
        error = clEnqueueReadBuffer(opencl_queue, buffer_to_read_from, CL_FALSE, 0, SIZE_BYTES, result_buffer, 0, NULL, NULL);
        clFinish(opencl_queue);
        stop_perf_measurement(&read_perf);
}

void setup_cl_compute() {
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
        update_kernel = clCreateKernel(program, "update", &error);
        checkError(error, "clCreateKernel");
        stop_perf_measurement(&program_perf);

	// Create the data objects
        //cl_mem in_buffer, out_buffer;
        start_perf_measurement(&create_perf);
        in_buffer = clCreateBuffer(opencl_context, CL_MEM_READ_ONLY, SIZE_BYTES, NULL, &error);
        clFinish(opencl_queue);
        stop_perf_measurement(&create_perf);

        checkError(error, "clCreateBuffer");
        out_buffer = clCreateBuffer(opencl_context, CL_MEM_WRITE_ONLY, SIZE_BYTES, NULL, &error);
        checkError(error, "clCreateBuffer");
	
	// Set the kernel arguments
        /*start_perf_measurement(&update_perf);
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

        checkError(error, "clEnqueueNDRangeKernel");*/

}

void copy_data_to_device(float *in, float *out) {
	cl_int error;
	// Copy data to the device
	start_perf_measurement(&write_perf);
        error = clEnqueueWriteBuffer(opencl_queue, in_buffer, CL_FALSE, 0, SIZE_BYTES, in, 0, NULL, NULL);
        //clFinish(opencl_queue);
        //stop_perf_measurement(&write_perf);
        checkError(error, "clEnqueueWriteBuffer");
        //start_perf_measurement(&write_perf);
        error = clEnqueueWriteBuffer(opencl_queue, out_buffer, CL_FALSE, 0, SIZE_BYTES, out, 0, NULL, NULL);
        clFinish(opencl_queue);
        stop_perf_measurement(&write_perf);

        checkError(error, "clEnqueueWriteBuffer");
}	


void cleanup_cl() {
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


