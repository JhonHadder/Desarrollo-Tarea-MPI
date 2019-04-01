#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

void main(int argc, char *argv[]){

	//Inicializa el ambiente MPI
	MPI_Init(&argc, &argv);

	//Puntos aleatorios a graficar sobre el cuadrado y la circunferencia en el contenida
	double x,y;

	//Cantidad de puntos aletorios a utilizar
	int NPuntos = 100000000;

	//Variable para guardar el calculo final
	double PI;

	//Numero de puntos dentro de la circunferencia
	int NPuntoEnCirculo = 0;

	//La utilizamos durante la verificacion de si un punto cae dentro de la circunferencia
	double denPuntos;

	//rank
	int my_rank;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(my_rank == 0){

		srand(time(NULL));

		for (int i = 0; i < NPuntos; i++){

			//Se seleccionan aleateoriamente dos puntos dentro del cuadrado que contendra nuestro circulo
			x = (double)rand()/RAND_MAX;
			y = (double)rand()/RAND_MAX;

			//Se utiliza para calcular los puntos que cayeron dentro de la circunferencia de radio 1
			denPuntos = x*x+y*y;

			//Comprueba si un punto coordenado cae dentro del circulo con radio 1
			if (denPuntos <= 1){
				NPuntoEnCirculo++;
			}
		}

		/*
		Aqui hacemos uso de esta formula (4*m)/n la cual nos permite aproximar el valor de pi teniendo en cuenta el numero de 
		puntos total y el numero de puntos dentro de la circunferencia.
		*/
		PI = (double)(4*NPuntoEnCirculo)/NPuntos;
		printf("%s %f \n","Calculo Secuencial MPI\nPi = ", PI );
	}	

	MPI_Finalize();
}
