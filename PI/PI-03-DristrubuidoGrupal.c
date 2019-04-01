#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

void main(int argc, char *argv[]){

	/* start up MPI */
	MPI_Init(&argc, &argv);

	//Puntos aleatorios a graficar sobre el cuadrado y la circunferencia en el contenida
	double x,y;

	//Cantidad de puntos aletorios a utilizar
	int nPuntos = 100000000;

	//Variable para guardar el calculo final
	double PI;

	//Utilizada para tener en cuenta cuantas veces cae un punto dentro del circulo
	int NPuntoEnCirculo = 0;

	//La utilizamos durante la verificacion de si un punto cae dentro de la circunferencia
	double denPuntos;

	//Cantidad de puntos dentro del circulo
	int nPuCirculo = 0;

	//rank
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	//Numero de procesos
	int nProcesos;

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesos);

	//Hacemos un ramdom con un anadido que evite la aparicion de valores o semillas iguales.
	srand(time(NULL)+rand()%100);

	//Comprobamos si al menos tenemos 2 procesos para asegurar una ejecucion distribuida.
	if(nProcesos < 2){
		fprintf(stderr, "Debe ejecutar con 2 o mas procesos" );
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	//Iteracion de procesos
	for (int i = my_rank; i < nPuntos; i+=nProcesos){

		//Se seleccionan aleateoriamente dos puntos dentro del cuadrado que contendra nuestro circulo.
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;

		//Se utiliza para calcular los puntos que cayeron dentro de la circunferencia de radio 1
		denPuntos = x*x+y*y;

		//Comprueba si un punto coordenado cae dentro del circulo con radio 1
		if (denPuntos <= 1){
			nPuCirculo++;
		}
	}

	//SUMA totalmente los puntos: Usamos MPI_Reduce como funcion grupal.
	MPI_Reduce(&nPuCirculo, &NPuntoEnCirculo, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (my_rank == 0){
		PI = (double)(4*NPuntoEnCirculo)/nPuntos;
		printf("%s %f \n","Calculo distribuido grupal PI\nPi = ", PI );
	}
	/* shut down MPI */
	MPI_Finalize();
}
