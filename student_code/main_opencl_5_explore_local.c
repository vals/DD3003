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
size_t locals[6][3] = {{1,1,0},{2,2,0},{4,4,0},{8,8,0},{16,16,0},{256,1,0}};
size_t *local_dimensions;


void run (int argc, const char * argv[]);

int main (int argc, const char * argv[]) {
	local_dimensions = NULL;
	printf("Using local dimensions NULL.\n");
	run(argc, argv);
	
	for (int i=0; i<6; i++) {
		iterations = 0;
		local_dimensions = &(locals[i][0]);
		printf("Using local dimensions (%d,%d,%d).\n", (int)local_dimensions[0],(int)local_dimensions[1],(int)local_dimensions[2]);
		run(argc, argv);
	}
}


void run (int argc, const char * argv[]) {
	// Put your main code here
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
	// Put your update_cl code here, but update it to use the appropriate local dimensions
}

void read_back_data(cl_mem buffer_to_read_from, float *result_buffer) {
	// Put your read_back_data code here
}

void setup_cl_compute() {
	// Put your setup_cl_compute code here
}

void copy_data_to_device(float *in, float *out) {
	// Put your copy_data_to_device code here
}	


void cleanup_cl() {
	// Cleanup
	clReleaseMemObject(out_buffer);
	clReleaseMemObject(in_buffer);
	clReleaseKernel(update_kernel);
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


