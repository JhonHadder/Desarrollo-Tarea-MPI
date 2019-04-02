#define N 10000
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  /* start up MPI */
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //Comprobamos si al menos tenemos 2 procesos para asegurar una ejecucion distribuida.
  if (world_size < 2) {
    fprintf(stderr, "Debe ejecutar con 2 o mas procesos %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  //Valores necesarios para el calculo de la integral.
  //Las letras son las generalmente utilizadas en el calculo integral.
  double sumaInt, a, b, w;
  //Arreglo en el que almacenaremos el valor de la función en cada punto del intervalo.
  //Reemplaza los xi
  double array[N];
  //Intervalo o rango de valores en el que realizaremos la Integral
  a = 0.0; b = 1.0;

  //Funcion el area del rectangulo que como sabemos nos permite:
  //Por medio de rectangulos encontrar el area bajo la curva de una funcion.
  w = (b-a)/N;
  sumaInt = 0.0;

  if (world_rank == 0) {

    //Llenamos primero el intervalo de valores en el que realizaremos nuestra integral.
    for(int i = 1; i <= N; i++){
      array[i] = a + (i-1)*w;
    }

    for(int i = 1; i < world_size; i++){
      //SEND
      MPI_Send(
        /* data         = */ &array,
        /* count        = */ N,
        /* datatype     = */ MPI_DOUBLE,
        /* destination  = */ i,
        /* tag          = */ 0,
        /* communicator = */ MPI_COMM_WORLD);
      }
      for (int i = 1; i <= (N/world_size); i++) {
        sumaInt = sumaInt + (w * (array[i] * array[i] * array[i]));
      }

      for(int i = 1; i < world_size; i++){
        double sumaInt1 = 0.0;
        //RECEIVE
        MPI_Recv(
          /* data         = */ &sumaInt1,
          /* count        = */ 1,
          /* datatype     = */ MPI_DOUBLE,
          /* source       = */ i,
          /* tag          = */ 0,
          /* communicator = */ MPI_COMM_WORLD,
          /* status       = */ MPI_STATUS_IGNORE);
          sumaInt += sumaInt1;
        }
        //Mostramos el valor calculado de la integral en el intervalo propuesto.
        printf("Valor retornado por la Integral = = %f \n",sumaInt);

      } else {
        //RECEIVE
        MPI_Recv(
          /* data         = */ &array,
          /* count        = */ N,
          /* datatype     = */ MPI_DOUBLE,
          /* source       = */ 0,
          /* tag          = */ 0,
          /* communicator = */ MPI_COMM_WORLD,
          /* status       = */ MPI_STATUS_IGNORE);
          for (int i = world_rank*(N/world_size); i <= (world_rank+1)*(N/world_size); i++) {
            sumaInt = sumaInt + (w*(array[i]*array[i] *array[i]));
          }

          //SEND
          MPI_Send(
            /* data         = */ &sumaInt,
            /* count        = */ 1,
            /* datatype     = */ MPI_DOUBLE,
            /* destination  = */ 0,
            /* tag          = */ 0,
            /* communicator = */ MPI_COMM_WORLD);
          }
          /* shut down MPI */
          MPI_Finalize();
        }
