#include <math.h>
#include <mpi.h>
#include <stdio.h>

//Polinomio sobre el que vamos a trabajar.
#define RAIZ(x) (x - 4)

int main(int argc, char** argv) {

  /* start up MPI */
  MPI_Init(NULL, NULL);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 2) {
    //Comprobamos si al menos tenemos 2 procesos para asegurar una ejecucion distribuida.
    fprintf(stderr, "Debe ejecutar con 2 o mas procesos %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  //Limites
  float xPositivo, xNegativo;
  //Raiz
  float xRaiz;
  //variables auxiliares
  float xa, ea;
  int i;
  //Variables auxiliares inicializadas.
  xa = 0;
  ea = 0;
  //Iterador
  i = 0;

  if (rank == 0) {
    //Limite superior en el plano.
    xPositivo = 1000000;
    //Limite Inferior en el plano.
    xNegativo = -1000000;
    int x = fabs(xPositivo) + fabs(xNegativo);
    x = x / size;
    xPositivo = xNegativo + x;
    int xt = xNegativo;

    for (int j = 1; j < size; j++) {
      xt += x;
      //Tramo del plamo en el que se trabaja actualmente
      int tramo[2] = { xt, xt + x };
      //SEND
      MPI_Send(
        /* data         = */ &tramo,
        /* count        = */ 2,
        /* datatype     = */ MPI_INT,
        /* destination  = */ j,
        /* tag          = */ 0,
        /* communicator = */ MPI_COMM_WORLD
      );
    }
  }
  else {
    //Tramo del plamo en el que se trabaja actualmente
    int tramo[2];
    //RECEIVE
    MPI_Recv(
      /* data         = */ &tramo,
      /* count        = */ 2,
      /* datatype     = */ MPI_INT,
      /* source       = */ 0,
      /* tag          = */ 0,
      /* communicator = */ MPI_COMM_WORLD,
      /* status       = */ MPI_STATUS_IGNORE
    );

    xNegativo = tramo[0];
    xPositivo = tramo[1];
  }

  //Calculo de la mitad de nuestros limites establecidos.
  //Segun el resultado (negativo, positivo).
  //Se procede a la seleccion de un nuevo rango segun el resultado obtenido.
  do {
    i++;
    xRaiz = (xPositivo + xNegativo) / 2;
    if(i >= 2) {
      ea = ((xRaiz - xa) / xRaiz) * 100;
    }
    ea = fabs(ea);
    xa = xRaiz;
    if(RAIZ(xRaiz) > 0) {
      xPositivo = xRaiz;
    } else {
      xNegativo = xRaiz;
    }
  } while(ea > 0.2 || (ea == 0.0 && i == 1));

  if (RAIZ(xRaiz) > -0.1 && RAIZ(xRaiz) < 0.1) {
    printf("\nLa raiz encontrada para la funcion f(x) = x - 4, es: %f\n", xRaiz);
  }
  /* shut down MPI */
  MPI_Finalize();

  return 0;
}//FIN
