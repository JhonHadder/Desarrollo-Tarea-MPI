#define N 10000
#include <time.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  /* start up MPI */
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  /* find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //Comprobamos si al menos tenemos 2 procesos para asegurar una ejecucion distribuida.
  if (world_size < 2) {
    fprintf(stderr, "Debe ejecutar con 2 o mas procesos %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  //Valores necesarios para el calculo de la integral.
  //Las letras son las generalmente utilizadas en el calculo integral.
  double a, b, w, sumInt;

  //Arreglo en el que almacenaremos el valor de la función en cada punto del intervalo.
  //Reemplaza los xi
  double array[N];
  //Definimos las variables que nos ayudan en el uso de funciones grupales
  int intervalo = (N/world_size);
  double array_Scatter[intervalo];

  //Intervalo o rango de valores en el que realizaremos la Integral
  a = 0.0; b = 1.0;

  //Funcion el area del rectangulo que como sabemos nos permite:
  //Por medio de rectangulos encontrar el area bajo la curva de una funcion.
  w = (b-a)/N;
  sumInt = 0.0;

  //Llenamos primero el intervalo de valores en el que realizaremos nuestra integral.
  for(int i = 0;i < N;i++){
    array[i] = a + (i-1)*w;
  }
 //Usamos MPI_Scatter como funcion grupal.
 MPI_Scatter(
   /* sendbuf      = */&array,
   /* sendcount    = */intervalo,
   /* sendtype     = */MPI_DOUBLE,
   /* recvbuf      = */array_Scatter,
   /* recvcount    = */intervalo,
   /* recvtype     = */MPI_DOUBLE,
   /* root         = */0,
   /* comm         = */MPI_COMM_WORLD);
 double sumInt_i = 0.0;
 for(int i = (world_rank*intervalo); i < ((world_rank * intervalo) + intervalo); i++){
   sumInt_i = sumInt_i  + (w * (array[i] * array[i] * array[i]));
 }

 double my_sums[world_size];

//Usamos MPI_Gather como funcion grupal.
MPI_Gather(
  /* sendbuf      = */&sumInt_i,
  /* sendcount      = */1,
  /* sendtype      = */MPI_DOUBLE,
  /* recvbuf      = */my_sums,
  /* recvcount      = */1,
  /* recvtype      = */MPI_DOUBLE,
  /* root      = */0,
  /* comm      = */MPI_COMM_WORLD);
if (world_rank == 0) {
  for(int i = 0;i < world_size; i++){
    sumInt = sumInt + my_sums[i];
  }
 }
/* shut down MPI */
 MPI_Finalize();
}
