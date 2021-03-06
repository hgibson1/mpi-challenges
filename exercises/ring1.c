//#include <stdio.h>
#include "mpi.h"

main(int argc, char** argv){

  int my_rank, my_size;
  int token;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &my_size);

    // All processes except 0 start blocking receive, wait for matching data
    // Process 1 starts receive first, then 2, 3, 4, ...
  if (my_rank == 0) {
    token = -1;
    MPI_Send(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (my_rank == my_size - 1) {
    MPI_Recv(&token, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("Process %d received token %d from process %d\n", my_rank, token, my_rank - 1);
  } else {
    MPI_Recv(&token, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&token, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
    printf("Process %d received token %d from process %d\n", my_rank, token, my_rank - 1);
  }
   
    // Now process 0 can receive from the last process.
   if (my_rank == 0) {
     MPI_Recv(&token, 1, MPI_INT, my_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     printf("Process %d received token %d from process %d\n", my_rank, token, my_size - 1);
   }

  MPI_Finalize();

}
