/*
 *  opencl_utils.c
 *  OpenCL Tutorial
 *
 *  Created by David Black-Schaffer on 07/07/2011.
 *  Copyright 2011 Uppsala University. All rights reserved.
 *
 */

#include "opencl_utils.h"

cl_device_type parse_device_type(int argc, const char * argv[]) {
	cl_device_type device_to_use = DEFAULT_DEVICE_TYPE;
	if (argc == 2) {
		if (!strcmp(argv[1], "GPU")) {
			printf("Selecting Graphics Processor (GPU).\n");
			device_to_use = CL_DEVICE_TYPE_GPU;
		} else if (!strcmp(argv[1], "CPU")) {
			printf("Selecting Central Processor (CPU).\n");
			device_to_use = CL_DEVICE_TYPE_CPU;
		} else {
			printf("Please specify CPU or GPU for device.\n");
			exit(-1);
		}
	} else {
		printf("Using default OpenCL device.\n");
	}
	return device_to_use;
}


void setup_cl(int argc, const char * argv[], cl_device_id *device, cl_context *context, cl_command_queue *queue) {
	cl_device_type device_to_use = parse_device_type(argc, argv);
	if (!init_opencl(device_to_use, device, context, queue)) {
		printf("ERROR initializing OpenCL.\n");
		exit(-1);
	}
}


int init_opencl (cl_device_type device_type, cl_device_id *device, cl_context *context, cl_command_queue *queue) {
	cl_int error;
	
	// Build a string description of the device type we are looking for.
	char device_type_name[100];
	if (device_type == CL_DEVICE_TYPE_CPU)
		strcpy(device_type_name, "CPU");
	else if (device_type == CL_DEVICE_TYPE_GPU)
		strcpy(device_type_name, "GPU");
	else if (device_type == CL_DEVICE_TYPE_DEFAULT)
		strcpy(device_type_name, "DEFAULT");
	else 
		strcpy(device_type_name, "(OTHER?)");
	printf("OpenCL: Looking for %s devices.\n", device_type_name);
	
	// Get all the platforms
	cl_platform_id platforms[10];
	cl_uint number_of_platforms_present;
	error = clGetPlatformIDs(10, platforms, &number_of_platforms_present);
	checkError(error, "clGetPlatformIDs");
	printf("OpenCL: Found %d platforms.\n", number_of_platforms_present);
	
	// For each platform look for the first device type that matches
	cl_device_id found_devices[10];
	cl_uint number_of_devices_found;
	for (cl_uint i=0; i<number_of_platforms_present; i++) {
		// Get the platform name
		char platform_name[1000];
		error = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 1000, platform_name, NULL);
		checkError(error, "clGetPlatformInfo");
		
		// Get all devices of the correct type for this platform (we only need one)
		error = clGetDeviceIDs(platforms[i], device_type, 10, found_devices, &number_of_devices_found);
		if (error != CL_DEVICE_NOT_FOUND) // NVIDIA returns an invalid error here.
			checkError(error, "clGetDeviceIDs");
		printf("OpenCL: Platform \"%s\" has %d devices of type %s.\n", platform_name, number_of_devices_found, device_type_name);
		
		// If we found a matching device then we're done looking
		if (number_of_devices_found > 0)
			break;
	}
	
	// Make sure we have a device!
	if (number_of_devices_found == 0) {
		printf("OpenCL: Found 0 matching devices.\n");
		return 0;
	}
	printf("Note: Picking the last device.\n");
	*device = found_devices[number_of_devices_found-1];
	
	// Print the name and vendor of the device
	char device_name[1000];
	error = clGetDeviceInfo(*device, CL_DEVICE_NAME, 1000, device_name, NULL);
	checkError(error, "clGetDeviceInfo");
	char device_vendor_name[1000];
	error = clGetDeviceInfo(*device, CL_DEVICE_VENDOR, 1000, device_vendor_name, NULL);
	checkError(error, "clGetDeviceInfo");
	printf("OpenCL: Selected \"%s\" from \"%s\".\n", device_name, device_vendor_name);
	
	// Create our context for the device
	printf("OpenCL: Creating a context for the device.\n");
	*context = clCreateContext(NULL, 1, device, NULL, NULL, &error);
	checkError(error, "clCreateContext");
	
	// Create our command queue for the device
	printf("OpenCL: Creating a command queue for the device with profiling enabled.\n");
	*queue = clCreateCommandQueue(*context, *device, CL_QUEUE_PROFILING_ENABLE, &error);
	checkError(error, "clCreateCommandQueue");
	
	// Done
	printf("OpenCL: Initialization done.\n");
	return 1;
}

char* load_source_file(const char *filename) {
	char *data;
	FILE *file;
	file = fopen(filename, "r");
	struct stat stat_info;
	int error = stat(filename, &stat_info);
	if (error) {
		printf("load_source_file: stat failed on file %s: %d\n", filename, error);
		exit(1);
	}
	
	data = (char*)malloc(stat_info.st_size+1);
	memset(data, 0, stat_info.st_size+1);
	
	size_t result = fread(data, stat_info.st_size, 1, file);
	if (result != 1) {
		printf("load_source_file: fread failed, returned %d\n", (int)result);
		exit(1);
	}
	return data;
}


void checkError(int error, const char *function) {
	if (error != CL_SUCCESS) {
		printf("%s failed: %d\n", function, error);
		exit(-1);
	}
}

void count_clevent_perf(cl_event evt, perf *t) {
	cl_int error;
	cl_ulong cl_start_time, cl_end_time;
	error = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &cl_start_time, NULL);
	checkError(error, "clGetEventProfilingInfo");
	error = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &cl_end_time, NULL);
	checkError(error, "clGetEventProfilingInfo");
	
	cl_ulong execution_time_ns = (cl_end_time-cl_start_time);
	
	double time = (double)execution_time_ns*1e-9;

	t->samples[t->current_sample++] = time;
	if (t->current_sample >= MAX_PERF_SAMPLES) {
		printf("Too many samples.");
		t->current_sample = MAX_PERF_SAMPLES-1;
	}
//	clReleaseEvent(evt);
}

void print_event_info(cl_event evt) {
	cl_int error;
	cl_ulong cl_start_time, cl_end_time, queued, submitted;
	error = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &cl_start_time, NULL);
	checkError(error, "clGetEventProfilingInfo");
	error = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &cl_end_time, NULL);
	checkError(error, "clGetEventProfilingInfo");
	error = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued, NULL);
	checkError(error, "clGetEventProfilingInfo");
	error = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &submitted, NULL);
	checkError(error, "clGetEventProfilingInfo");
	
	printf("submit->queued: %f ms, queued->start: %f ms, start->end: %f ms\n", 
		   (submitted-queued)*1e-6, (cl_start_time-submitted)*1e-6, (cl_end_time-cl_start_time)*1e-6);
}

