## Report for lab on OpenMP

1. I simply modified the two existing print statements in the
source file so that they read `printf("Starting on %i threads\n", omp_get_max_threads());`
and `printf("Hello World from thread %i!\n", omp_get_thread_num());`, respectively.

2. I predict it will print 8 copies of "a= 110 b= 10"... It did not, it printed
8 copies of "a= 10 b= 10. Ok, I see, the variable set as private is not _really_
the one which got assigned to 100 before we set it private; it got cleared
when it was set as private.

When using `firstprivate` the results seem more sensible.
The `lastprivate` command will set the original `a` to the
last value it is set to in a thread after they have all finished.
It might be useful for example when performing some local sumation
and one want the last pertial sum to have been applied.

3. When doubling the threads, starting from 1, one notices that
the runtime decreases (from 8.8 seconds) up until one reaches
128 threads (1.10 seconds), after which the execution time
starts increasing again.

I found `guided` with chunk size `1` to be quickest
at 1.126866 (for 8 threads).

4. I changed most of the internal of main in to

```

  sum=0.0;

#pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n ; i++) { 
      sum = sum + A[i];
    }


  printf("Global sum is: %f\n",  sum);
```

The result was the same as the given program; 499999500000.000000.
(And indeed, this is the sum of 1, ... ,999999.)

5. I simply added the line

```
#pragma omp parallel for reduction(+:sum) private(x)
```

above the for loop in `pi.c`. When I didn't have the `private(x)` part
I got different results every time I ran the program. 

For the sorting: Serial: 6.2 seconds; Outer loop parallelized by
`#pragma omp parallel for private(rank, i)`: 0.8 seconds; Inner loop
parallelized by `#pragma omp parallel for reduction(+:rank)`:
 8.1 seconds. Nested with 2 outer, 4 inner threads: 7.8 seconds;
 Nested with 4 outer, 2 inner threads: 2.9 seconds. So just focusing on
 the outer loop is most gainful.
 
 6. I added the line `!$omp parallel do` as line 26 (before the first internal
 loop of the factorization). And the line `!$omp parallel do private(i)`
 as line 30 (before the second internal loop). This reduces the run time from
 8.3 seconds to 3.1 seconds; the error is the same.
 
 We start by setting a lock for each column during the initiation.
 The calculation for the first column is performed outside the main
 loop, after which the column is unlocked. This is what happens
 for every column in the loop; after the writing is done, the
 lock is unset so that other threads can use the data.
 So threads will only need to wait for each column before it is unlocked.