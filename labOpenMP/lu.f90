program lu

  implicit none
  integer, parameter :: DP=kind(0.0D0),n=2000

!--  Variables
  integer :: i,j,k,nthr,thrid
  real(kind=DP),dimension(n,n) :: A,B,L,U
  real(kind=DP) :: timer,error
  integer :: OMP_GET_MAX_THREADS
  real(kind=DP) :: walltime  

  nthr=OMP_GET_MAX_THREADS()
  timer=walltime()

  
!-- Initate matrix
  do j=1,n
     do i=1,n
        A(i,j)=1.0/(i+j)
     end do
  end do

!-- LU-factorize
  do k=1,n
!$omp parallel do
     do i=k+1,n
        A(i,k)=A(i,k)/A(k,k)
     end do
!$omp parallel do private(i)
     do j=k+1,n
        do i=k+1,n
           A(i,j)=A(i,j)-A(i,k)*A(k,j)
        end do
     end do
  end do

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

