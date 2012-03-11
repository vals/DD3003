/**********************************************************************
 * This program calculates performs quicksort using C 
 * (with multiple threads, eventually)
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS     2

typedef struct a1
{
    int l;
    int r;
    int p;
    int a[];
} qs_param;

void *thread_quicksort(void *arg);
int partition( int[], int, int);

int main() 
{
    int parallel_recursion_level = 2;
    int a[] = { 7, 12, 1, -2, 0, 15, 4, 11, 9};

    int i;
    printf("\nUnsorted array:\t");
    for(i = 0; i < 9; ++i)
        printf(" %d ", a[i]);

    quicksort( a, 0, 8);

    printf("\n\nSorted array:\t");
    for(i = 0; i < 9; ++i)
        printf(" %d ", a[i]);

    printf("\n\n");

    return 1;
}

void *thread_quicksort(void *arg)
{
    qs_param *qp;
    qp = (qs_param *) arg;
    int a = qp->a;
    int l = qp->l;
    int r = qp->r;
    int p = qp->p;
    
    int j;

    if( l < r ) 
    {
        j = partition(a, l, r);

        qs_param *qp_left;
        qp_left->a = a;
        qp_left->l = l;
        qp_left->r = j - 1
        qp_left->p = p - 1

        qs_param *qp_right;
        qp_right->a = a;
        qp_right->l = j + 1;
        qp_right->r = r;
        qp_right->p = p - 1;

        if(p > 0)
        {
            pthread_create(&thread[2*(p - 1)], NULL, 
                thread_quicksort, (void*)%qp_left);
            pthread_create(&thread[2*(p - 1) + 1], NULL, 
                thread_quicksort, (void*)%qp_left);
        }
        else
        {
            quicksort(a, l, j - 1, 0);
            quicksort(a, j + 1, r, 0);
        }
    }

}

int partition( int a[], int l, int r)
{
    int pivot, i, j, t;
    pivot = a[l];
    i = l; j = r+1;

    while(1)
    {
        do ++i; while( a[i] <= pivot && i <= r );
        do --j; while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    t = a[l];
    a[l] = a[j];
    a[j] = t;

    return j;
}
