program lu

  implicit none
  integer, parameter :: DP=kind(0.0D0),n=2000

!--  Variables
  integer :: i,j,k,nthr,thrid,chunk=1
  real(kind=DP), dimension(n,n) :: A,B,L,U
  real(kind=DP) :: timer,error,walltime
  integer(kind=8), dimension(n)::lck
  integer::omp_get_thread_num,omp_get_max_threads
  nthr=omp_get_max_threads()
  
!-- Set up locks for each column
  do i=1,n
     call omp_init_lock(lck(i))
  end do

  timer=walltime()
!$OMP PARALLEL PRIVATE(i,j,k,thrid) 
  thrid=omp_get_thread_num();

!-- Initate matrix
!$OMP DO SCHEDULE(STATIC,chunk)
  do j=1,n
     do i=1,n
        A(i,j)=1.0/(i+j)
     end do
     call omp_set_lock(lck(j))
  end do
!$OMP END DO

!-- First column of L
  if (thrid==0) then
     do i=2,n
        A(i,1)=A(i,1)/A(1,1)
     end do
     call omp_unset_lock(lck(1))     
  end if

!-- LU-factorization
  do k=1,n
!     write(*,*) k,thrid
     call omp_set_lock(lck(k))
     call omp_unset_lock(lck(k))
!$OMP DO SCHEDULE(STATIC,chunk)
     do j=1,n
        if (j>k) then 
           do i=k+1,n
              A(i,j)=A(i,j)-A(i,k)*A(k,j)
           end do
           if (j==k+1) then
              do i=k+2,n
                 A(i,k+1)=A(i,k+1)/A(k+1,k+1)
              end do
              call omp_unset_lock(lck(k+1))
           end if
        end if
     end do
!$OMP END DO NOWAIT
  end do

!$OMP END PARALLEL
  timer=walltime()-timer

  write(*,*) 'n = ',n,'   time = ',timer,'   nthr = ',nthr

! CHECK CORRECTNESS
  do j=1,n
     L(j,j)=1
     U(j,j)=A(j,j)
     do i=j+1,n
        L(i,j)=A(i,j)
        U(i,j)=0
     end do
     do i=1,j-1
        U(i,j)=A(i,j)
        L(i,j)=0
     end do
  end do
  B=0
  do j=1,n
     do k=1,n
        do i=1,n
           B(i,j)=B(i,j)+L(i,k)*U(k,j)
        end do
     end do
  end do
  error=0.0
  do j=1,n
     do i=1,n
        error=error+abs(1.0/(i+j)-B(i,j))
     end do
  end do

  write(*,*) 'ERROR: ',error

end program lu

