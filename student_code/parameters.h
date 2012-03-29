/*
 *  parameters.h
 *  OpenCL Tutorial
 *
 *  Created by David Black-Schaffer on 07/07/2011.
 *  Copyright 2011 Uppsala University. All rights reserved.
 *
 */

// Device to use if neither CPU nor GPU are specified on the command line
#define DEFAULT_DEVICE_TYPE CL_DEVICE_TYPE_CPU

// Range limit, arbitrary. (E.g., data is created to fit this range.)
#define LIMIT 100
// Increase to start with a larger initial range
#define INITIAL_RANGE_MULTIPLE 4
#define BIG_RANGE LIMIT*INITIAL_RANGE_MULTIPLE*100
// Data size. Use about 1024*7 for 196MB x 2, 1024*5 for 100MBx2
#define SIZE 1024*4
#define SIZE_BYTES sizeof(float)*SIZE*SIZE

// Define to 1 to not include first and last measurements in performance statistics
#define PERF_IGNORE_FIRST_LAST	1
#define MAX_PERF_SAMPLES 100
