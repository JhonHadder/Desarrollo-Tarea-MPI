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

    //rank
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //Numero de procesos
	int nProcesos;
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesos);


	//Comprobamos si al menos tenemos 2 procesos para asegurar una ejecucion distribuida.
	if(nProcesos < 2){
		fprintf(stderr, "Debe ejecutar con 2 o mas procesos" );
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	if (my_rank == 0){

		for (int i = 1; i < nProcesos; i++){

			//La utilizamos para comprobar el numero de puntos que caen dentro de la circunferencia por cada proceso ejecutado
			int procDenPunto = 0;
			
			//RECEIVE de los puntos encontrados dentro de la circunferencia
			MPI_Recv(
			/* data         = */&procDenPunto, 
			/* count        = */1, 
			/* datatype     = */ MPI_INT, 
			/* source       = */i, 
			/* tag          = */0, 
			/* communicator = */MPI_COMM_WORLD, 
			/* status       = */MPI_STATUS_IGNORE
			);

			NPuntoEnCirculo += procDenPunto;
		}

		PI = (double)(4*NPuntoEnCirculo)/nPuntos;
		printf("%s %f \n","Calculo distribuido MPI\nPi = ", PI );
	}
	else{

		//Numero de puntos dentro del circulo
		int nPuCirculo = 0;

		for (int i = 0; i < nPuntos/(nProcesos-1); i++){

			//Se inicializa el random con un factor de desempate, en caso de semillas iguales
			srand(time(NULL)+rand()%(i+2));

			//Se seleccionan aleateoriamente dos puntos dentro del cuadrado que contendra nuestro circulo
			x = (double)rand()/RAND_MAX;
			y = (double)rand()/RAND_MAX;

			//Se utiliza para calcular los puntos que cayeron dentro de la circunferencia de radio 1
			denPuntos = x*x+y*y;

			//Comprueba si un punto coordenado cae dentro del circulo con radio 1
			if (denPuntos <= 1){
				nPuCirculo++;
			}
		}

		//SEND de los puntos encontrados dentro de la circunferencia
		MPI_Send(
		/* data         = */&nPuCirculo,
		/* count        = */1,
		/* datatype     = */MPI_INT, 
		/* destination  = */0, 
		/* tag          = */0,
		/* communicator = */MPI_COMM_WORLD
		);
	}
    /* shut down MPI */
	MPI_Finalize();
}
