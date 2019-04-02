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
  xPositivo = 1000000;
  xNegativo = -1000000;
  xa = 0;
  ea = 0;
  //Iterador
  i = 0;

  //Division del plano
  int x = fabs(xPositivo) + fabs(xNegativo);
  x = x / size;

  //Usamos tramos como en el distribuido.
  double tramo[size];
  for (int j = 0; j < size + 1; j++) {
    tramo[j] = xNegativo;
    xNegativo += x;
  }

  double tramoScatter[x];
  //Usamos MPI_Scatter como funcion grupal.
  MPI_Scatter(
    /* sendbuf        = */&tramo,
    /* sendcount      = */size,
    /* sendtype       = */MPI_DOUBLE,
    /* recvbuf        = */tramoScatter,
    /* recvcount      = */size,
    /* recvtype       = */MPI_DOUBLE,
    /* root           = */0,
    /* comm            = */MPI_COMM_WORLD
  );

  xPositivo = tramo[rank + 1];
  xNegativo = tramo[rank];
  xa = 0;
  ea = 0;
  i = 0;

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

  if (RAIZ(xRaiz) > -0.1 && RAIZ(xRaiz) < 0.1){
    printf("\nLa raiz encontrada para la funcion f(x) = x - 4, es: %f\n", xRaiz);
  }
  /* shut down MPI */
  MPI_Finalize();

  return 0;
}//FIN
