/*
 *  opencl_utils.h
 *  OpenCL Tutorial
 *
 *  Created by David Black-Schaffer on 07/07/2011.
 *  Copyright 2011 Uppsala University. All rights reserved.
 *
 */

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "util.h"


cl_device_type parse_device_type(int argc, const char * argv[]);
void setup_cl(int argc, const char * argv[], cl_device_id *device, cl_context *context, cl_command_queue *queue);
int init_opencl (cl_device_type device_type, cl_device_id *device, cl_context *context, cl_command_queue *queue);
char* load_source_file(const char *filename);
void count_clevent_perf(cl_event evt, perf *perf);

void checkError(int error, const char *function);
void print_event_info(cl_event evt);
