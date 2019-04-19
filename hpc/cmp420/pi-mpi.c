#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#define pi 3.14159265358979323846

int main(int argc, char *argv[])
{
  double total, h, sum, x;
  double t1, t2;
  long long int i, n = 1e10;
  int rank, numprocs;
  char hostname[256];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  t1 = MPI_Wtime();

  h = 1./n;
  sum = 0.;

  gethostname(hostname,255);
  printf("Process number: %d out of %d on host %s\n", rank, numprocs, hostname);

  for (i = rank+1; i <= n; i += numprocs) {
    x = h * ( i - 0.5 );
    sum += 4.0 / ( 1.0 + pow(x,2));
  }

  sum *= h;
  MPI_Reduce(&sum,&total,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

  if (rank == 0) {
    t2 = MPI_Wtime();
    printf("Elapsed time is %f\n", t2 - t1 );
    printf("%.17g  %.17g\n", total, fabs(total-pi));
  }

  MPI_Finalize();
  return 0;
}
