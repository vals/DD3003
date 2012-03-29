# Lab report on OpenCL
## Paul Costea, Valentine Svensson, Fernanda Villegas

On the Sun computer we used in the lab we had to convert the spreadsheet to
Open Document format in order to edit it. Hopefully it will be openable by
the person correcting it!

We managed to do the first four section of the lab, and were starting on the
fifth one while we were waiting to be able to ask some questions about the
data movement related to the questions in task 4.

Also, (as is probably known,) the implementation of OpenCL on the GPUtin
nodes was the one by nVidia, so there was no support for running the
programs in CPU mode.

### 0.

At the time we didn't see what the GPU was, so we didn't put this in the
zpreadsheet, but from the UPPMAX user guide on Tintin we can see that the
GPU's are nVidia Tesla S2050.

### 1.

No real comments aside from what we wrote in the spreadsheet. Though for the
sake of feedback, copying numbers from the terminal to the Open Office
spreadsheet software on the Sun machines is horrible! Using keyboard shurtcuts
between the places doesn't work; just marking text as in e g the mac or
ubuntu terminals to copy doesn't either. We had to mark text, right click,
click "Copy", go to the spreadsheet, right click, click "Paste" for each
number. It might sound like a silly thing to complain about, but it actually
took several minutes from every task.

### 2.

We looked for the functions which seemed to do what the perf variables
did. The source file is quite long, so in stead, we post here the relevant
part of the result from `diff main_opencl_1.c main_opencl_2_profile.c`, where
one can see at which file lines the perfs have been started and stopped.

    <       start_perf_measurement(&update_perf);
    42d37
    <       stop_perf_measurement(&update_perf);
    62a58
    > 
    73a70
    >   
    74a72
    >   start_perf_measurement(&program_perf);
    94c92,94
    <   
    ---
    >   stop_perf_measurement(&program_perf);
    > 
    > 
    96a97
    >   start_perf_measurement(&create_perf);
    97a99,100
    >   stop_perf_measurement(&create_perf);
    > 
    102a106
    >   start_perf_measurement(&write_perf);
    103a108,109
    >   stop_perf_measurement(&write_perf);
    > 
    104a111,112
    > 
    >   start_perf_measurement(&write_perf);
    105a114,115
    >   stop_perf_measurement(&write_perf);
    > 
    108a119
    >   start_perf_measurement(&update_perf);
    112a124
    >   stop_perf_measurement(&update_perf);
    115c127,129
    <   size_t global_dimensions[] = {SIZE,SIZE,0};
    ---
    >   size_t global_dimensions[] = {SIZE,SIZE,0}; // Ignore the border
    > 
    >   start_perf_measurement(&update_perf);
    117c131
    <   checkError(error, "clEnqueueNDRangeKernel");
    ---
    >   stop_perf_measurement(&update_perf);
    118a133,134
    >   checkError(error, "clEnqueueNDRangeKernel");
    >   
    119a136
    >   start_perf_measurement(&read_perf);
    120a138,139
    >   stop_perf_measurement(&read_perf);
    > 
    123a143
    >   start_perf_measurement(&finish_perf);
    124a145,146
    >   stop_perf_measurement(&finish_perf);
    >   
    125a148
    > 
    127a151
    >   start_perf_measurement(&cleanup_perf);
    133c157
    < }
    ---
    >   stop_perf_measurement(&cleanup_perf);
    134a159
    > }

### 3.

As the task specified, we added clFinish in various places. Again, we include
(the relevant part of) a `diff` between `main_opencl_2_profile.c` and our
`main_opencl_3_profile_finish.c`.

    <   // Load the program source
    ---
    >   /*// Load the program source
    92c129
    <   stop_perf_measurement(&program_perf);
    ---
    >   stop_perf_measurement(&program_perf);*/
    98a136
    >   clFinish(opencl_queue);
    107a146
    >   clFinish(opencl_queue);
    113a153
    >   clFinish(opencl_queue);
    130a171
    >   clFinish(opencl_queue);
    137a179
    >   clFinish(opencl_queue);
    154c196
    <   clReleaseKernel(kernel);
    ---
    >   /*clReleaseKernel(kernel);
    156c198
    <   free(program_text);
    ---
    >   free(program_text);*/

For question **G.** we removed the OpenCL setup from the loop, not anticipating
this would be part of task 4.

### 4.

For the sake of simplicity, I will simply paste in the function implementations
we did.

#####  `copy_data_to_device`
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

##### `update_cl`
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

##### `read_back_data`
    void read_back_data(cl_mem buffer_to_read_from, float *result_buffer) {
        cl_int error;
        // Enqueue a read to get the data back
        start_perf_measurement(&read_perf);
            error = clEnqueueReadBuffer(opencl_queue, buffer_to_read_from, CL_FALSE, 0, SIZE_BYTES, result_buffer, 0, NULL, NULL);
            clFinish(opencl_queue);
            stop_perf_measurement(&read_perf);
    }

##### `cleanup_cl`
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

Of course, the code is mostly from the previous file, but we did have to be
careful to rename a couple of variables!

### Feedback

And that's as far as we got!

Before answering the questions, I need to also rant about some other annoying
oddities of the terminal in the Sun computer (or 'client'):
Backspace removed the character _to the right_ of the caret, which was
quite hindersome. And for some reason we could only remove one character at a
time in emacs.

On to the questions!

1.  c
2.  b (mostly due to system weirdness)
3.  d
4.  c
5.  e
6.  d
