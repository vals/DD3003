UTILS = opencl_utils.c util.c
CC = cc
CFLAGS = -Wall -Wno-comment -Os -std=gnu99
LAB_OPENCL_INC=/usr/local/cuda/include
ZORN_OPENCL_INC=/cfs/zorn/nobackup/opt/NVIDIA_GPU_Computing_SDK/OpenCL/common/inc


# Find out if we're on a mac
DARWIN = $(strip $(findstring DARWIN, $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])))
ifneq ($(DARWIN),)
	# Macs use frameworks
	LIBS = -framework OpenCL
else
	# Linux
	LIBS = -I$(LAB_OPENCL_INC) \
		-I$(ZORN_OPENCL_INC) \
		-lOpenCL \
		-lm
endif

all: hello 1 2 3 4 5 6 7

zip:
	zip tutorial.zip Makefile *.c *.h *.cl 

hello: hello_world.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} hello_world.c -o hello_world.run

1: main_c_1.c main_opencl_1.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} -O3 main_c_1.c -o 1_c.run
	${CC} ${CFLAGS} ${LIBS} ${UTILS} main_opencl_1.c -o 1_opencl.run

2: main_opencl_2_profile.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} $< -o 2_profile.run
	
3: main_opencl_3_profile_finish.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} $< -o 3_profile_finish.run
	
4: main_opencl_4_profile_nooverhead.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} $< -o 4_no_overhead.run

5: main_opencl_5_explore_local.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} $< -o 5_explore_local.run

6: main_opencl_6_range_kernel.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} $< -o 6_range_kernel.run

7: main_opencl_7_range_coalesced.c ${UTILS}
	${CC} ${CFLAGS} ${LIBS} ${UTILS} $< -o 7_range_coalesced.run

run:
	-date
	@echo "============== 1 C-CPU =============="
	-./1_c.run
	@echo "============== 1 OpenCL-CPU =============="
	-./1_opencl.run CPU
	@echo "============== 1 OpenCL-GPU =============="
	-./1_opencl.run GPU
	@echo "============== 2 OpenCL-CPU =============="
	-./2_profile.run CPU
	@echo "============== 2 OpenCL-GPU =============="
	-./2_profile.run GPU
	@echo "============== 3 OpenCL-CPU =============="
	-./3_profile_finish.run CPU
	@echo "============== 3 OpenCL-GPU =============="
	-./3_profile_finish.run GPU
	@echo "============== 4 OpenCL-CPU =============="
	-./4_no_overhead.run CPU
	@echo "============== 4 OpenCL-GPU =============="
	-./4_no_overhead.run GPU
	@echo "============== 5 OpenCL-GPU =============="
	-./5_explore_local.run GPU
	@echo "============== 6 OpenCL-CPU =============="
	-./6_range_kernel.run CPU
	@echo "============== 6 OpenCL-GPU =============="
	-./6_range_kernel.run GPU
	@echo "============== 7 OpenCL-CPU =============="
	-./7_range_coalesced.run CPU
	@echo "============== 7 OpenCL-GPU =============="
	-./7_range_coalesced.run GPU
	-date

clean:
	rm -f *.run
