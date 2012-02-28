Lecture 3:
----------

Exercise 9:

 1.  Let p denote performance, and I the instruction count (multiplied by some 
constant so we can use the GHz unit). Then
I * px = (clock rate of Px)/(CPI of Px), and so we can compare the three
processors; I * p1 = 2/1.5 = 40/30, I * p2 = 1.5/1 = 45/30, 
I * p3 = 3/2.5 = 36/30.
Meaning, compared between the three, P2 has the highest performance.
 2.  We get these by juggling around the various expressoins for performance
terms. To be brief, in order, number of cycles: 2 * 10^10, (3/2) * 10^10,
3 * 10^10, and number of instructions:
(4/3) * 10^10, (3/2) * 10^10, (6/5) * 10/10.
 3.  By manipulating the fact that 
0.7 * (CPU time) = (Instructions) * 1.2 * (CPI) / (New clock rate)
we get that
(New clock rate) = (12/7) * (Instructions) * (CPI)/(CPU time) = 
(12/7) * (Old clock rate).
So to reduce the CPU time by 30% we would need to increase the clock rate by
roughly 71%, so, in order, P1, P2, P3 would need to become 3.4 Ghz, 2.6 GHz
and 5.1 Ghz, respectively.

Exercise 10:

 1.  a. 200 - 0.2 * 35 = 193s b. 210 - 0.2 * 50 = 200s
 2.  a. 0.2 * 85 = 17s, b. 0.2 * 80 = 16s
 3.  No, even if branch instructions where eliminated completely there would
only be time reductions of 15% and 14% for a and b respectively.

Exercise 11:

 1.  Compute time ratios: [0.55, 0.51, 0.61, 0.47, 0.48],
Communication time ratios: [1.18, 1.31, 1.29, 1.05, 1.13].
 2.  Geometric means: 0.52 and 1.19. So we would suspect getting a computation
time of 0.52 * 6.5 = 3.38ms and the time for communication to be
26 * 1.19 = 30.94ms.
 3.  By the same extrapolation as above we can approximate the times for a
single processor as 176/0.52 = 338.46ms for computation and 11/1.19 = 9.24ms 
for communication.

Exercise 12:

 1.  _Ideally_; 2500, 1250, 625 requests/sec for 4, 8, 16 CPUs/cores 
respectively.
 2.  This would mean that _everything_ performed in in a 'request' is completely
parallelizable, which is quite unlikely.

Lecture 4:
----------
Exercise 13:

P4: Data dependent on S1, Data dependent on S3, Data dependent on S9 (loop carried), Data dependent on S10 (loop carried)
P6: data dependent on S5
S7: Data dependent on S2, Output dependent on S2, Data dependant on S5, Data dependent on S7 (loop carried), Output dependent on S7 (loop carried)
P8: Data dependent on S2, Data dependent on S7 (loop carried)
S10: Data dependent on S1, Data dependent on S10 (loop carried), Output dependent on S10 (loop carried)
S11: Data dependent on S2, Data dependent on S7

Exercise 14:

 1.  50 + 50 + 60 + 60 + 50 + 50 = 320 ns
 2.  100 * 320 = 32 000 ns = 32 µs
 3.  
     *  Latency does not decrease with pipelining, so 320 ns.
     *  In the first instruction we will have stage times of 55->55->65->65->55->10->55,
     in the second instruction we will have 55->55->10->65->65->55->10->55. After that
     it will follow the pattern 55->10->55->10->65->65->55->10->55 up until the 
     final instruction, where we will have the stage times
     55->10->55->10->65->65->55->55.
     So for 100 instructions, the third instruction will be initiated 110 ns
     from the first, so that the 100th will be 110 + 98 * 65 = 6480 ns later.
     The 100th instruction will take 370 ns, so in total, the 100 instructions
     will take 6850 ns = 6.85 µs.
     *  The speedup is N * 320 - (110 + N * 65 + 370) = N * 255 - 480 ns, where
     N is the number of instructions. Asymptotically, it will be 65 / 320 ≈
     0.2 times the time of the nonpipelined machine.

Exercise 15:

 1.  If we unroll the loop the number of break condition checks decreases.
 The loop overhead will consist of dividing the number of iterations with the 
 desired depth. If the unrolling depth is even, the body of the loop can be
 simplified so that if statement is not needed.
 2. 2 seem like a good place to start, we could then change the code so that
 we don't need the if statement, and thus not need to do the modulo check.
 For larger unrolling depth, we could check for the parity of the depth size
 to see wheter an if statement is needed, or soemthing in that vein. Having the
 depth a prime factor of 100 would also be nice.
 3.  
     n = 4
     do i = 1,100/4
       a((i - 1) * 4 + 1) = y
       a((i - 1) * 4 + 2) = x
       a((i - 1) * 4 + 3) = y
       a((i - 1) * 4 + 4) = x
    enddo
