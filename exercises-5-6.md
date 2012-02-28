#Lecture 5

####Exercise 16:
Quiksort works by dividing the list of items in two, and applying quicksort
to the parts. So in every dividing step we can do the two parts in parallel.
Thus, since it is the same task to different data, it will be data parallelism.

####Exercise 17:
A chess program works by simulating a tree of possible moves at a certain depth
from a given position, and assigning a score to the resulting state, then
performing the move which result in the highest score. Trying branches in
parallel would be a strategy, and this implies data parallelism.

####Exercise 18:
When we partition data in blocks we also get spatial locality of the data.
This means we wouldn't need to do as much synchronization of the data. Of
course this requires that the problem supports it. However, for some reason
one block might get done before the other, after which one processor is just
wasting time on waiting for the other(s) to finish. With cyclic data partition
we let a processor handle a smaller piece at a time. When we start to go out
of pieces, the idle processes shouldn't be idle for too long. It of course
means more synchronization (data transfer). Also it might be harder to
understand where what data is being processed when for example doing debugging.

Granularity refers to the ratio of computation compared to communication, this
is higher when we do cyclic data partition.

####Exercise 19:
There are only so many tasks to do in a problem, so increasing the number of
processors will not increase the performance of solving the problem unless
you do some data parallelism on the subproblems.

####Exercise 20:
An _embarrassingly parallel program_ is one that can be parallelized almost
without any synchronization what so ever. The slides mentions searching over
very large sets, where we just wish to get any matches we can find. Another
example would be image processing, for example applying a color filter; we can
split the image and perform the filter on each piece without bothering about
the rest of the image.

#Lecture 6

####Exercise 21:
 1. True
 2. False
 3. False
 4. False

####Exercise 22:
1. Not directly
2. Not directly

    When the black size becomes small, we can not take advantage of spatial
    locality of data. But when the block size becomes too large, cache misses
    will be more heavily punished. What we need to do is balance these.

3. Not really
4. This makes sense

    If we have higher bandwidth transferring more data should take the same
    time as a lesser amount would with lower bandwidth. Thus we could take
    better advantage about larger cache blocks.

####Exercise 23:
The first one is more generally true;
the L-1 cache is focused on minimal hit time, and uses smaller block sizes.

####Exercise 24:
1. a
2. c
3. b
4. d

####Exercise 25:
1. False
2. True
3. False

####Exercise 26:
1. In a) the references in the inner loop are temporally local. So the `i`'th
row and column of `A` and the first column of `B` in step `i`. In b) it will
be the latest number of column elements in `j`th column of `A`, and the latest
number of row elements in the `j`th row of `A`. As well as `B[j][0]`.
2. The row elements of `A`, in both cases (since it is row-major). For `B` we 
can not really say as we have no clue on how many columns it consists of.
Had it been 1-dimensional we could have called it's elements spatially local.
(This should be the same both in case a) and b)).
3. Temporal locality should not be any different. But since Fortran is
column-major we can say that the column elements of `A` are spatially local,
and also all of `B[][0]` exhibits spatial locality.
