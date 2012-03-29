#include "parameters.h"
#include "util.h"
#include "opencl_utils.h"


// Performance measurement
perf program_perf, create_perf, write_perf, read_perf, finish_perf, cleanup_perf;
perf total_perf, update_perf, range_perf;
perf reduction_perf;
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
void range_cl(cl_mem out_b);
cl_device_id opencl_device;
cl_context opencl_context;
cl_command_queue opencl_queue;
cl_program opencl_program;
cl_kernel update_kernel, range_kernel;
cl_mem in_buffer, out_buffer, range_buffer;

// Data for holding the range we read back
float *range_data;
// The number of work items to use to calculate the range
#define RANGE_SIZE 1024*4

int iterations = 0;

int main (int argc, const char * argv[]) {
	printf("6. Range Kernel\n");

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

		// Range
		start_perf_measurement(&range_perf);
		range_cl(get_out_buffer());
		stop_perf_measurement(&range_perf);
		
		// Read back the data
		start_perf_measurement(&read_perf);
		read_back_data(range_buffer, range_data);
		stop_perf_measurement(&read_perf);
		
		// Compute Range
		start_perf_measurement(&reduction_perf);
		range = find_range(range_data, RANGE_SIZE*2);
		stop_perf_measurement(&reduction_perf);
		
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
	// Put your update_cl code here
}

void read_back_data(cl_mem buffer_to_read_from, float *result_buffer) {
	// Put your read_back_data code here
}

void setup_cl_compute() {
	// Put your setup_cl_compute code here, but update it to create the range kernel and range_buffer
}

void copy_data_to_device(float *in, float *out) {
	// Put your copy_data_to_device code here
}	

void range_cl(cl_mem out_b) {
	cl_int error;
	// Set the kernel arguments
	
	// Enqueue the kernel
}



void cleanup_cl() {
	// Cleanup
	clReleaseMemObject(out_buffer);
	clReleaseMemObject(in_buffer);
	clReleaseMemObject(range_buffer);
	clReleaseKernel(update_kernel);
	clReleaseKernel(range_kernel);
	clReleaseProgram(opencl_program);
	clReleaseCommandQueue(opencl_queue);
	clReleaseContext(opencl_context);
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
	init_perf(&reduction_perf);
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
	printf(">Reduction:     ");
	print_perf_measurement(&reduction_perf);
}	


