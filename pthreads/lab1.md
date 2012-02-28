# Report from Pthreads lab


We added argument (void *)t to pthread_create, then in *HelloWorld we
made a long t that we assigned to (long) arg, which we then printed in a
a formatted string.


To make the hello2 print different values for different threads we modified
line 57 so that thread_data_array[t].message = messages[t].


Since we are setting the GlobalData[tid] = -tid in each thread, we will
have a race condition causing the sum in that particular thread to have a
value depending on which threads have been run before. And we can not know
the order of the threads.


If we set the attribute to PTHREAD_CREATE_DETACHED and remove the pthread_join
function the loop over the threads will print a message indicating the threads
are finished even though they are not. However, they will finish when we get
to pthread_exit(NULL), and join.

If we also remove pthread_exit(NULL) the threads will just abort when the
program finishes.


If we don't lock the mutexsum (but still use -O3) each thread will locally
sum the 100 000, but the threads starting after some thread have managed to
finish will read e.g. 100 000, and sum from that as a starting value of sum.

When we also remove -O3 from the compiler the value of sum is not cached, and
will be read and written in each (parallel) iteration. So that some results
from the summation will be overwritten by a thread slighly out of sync. Giving
us results like 115 403, where the ~15 403 corresponds to how much later the
non-first started.


If we remove the barrier