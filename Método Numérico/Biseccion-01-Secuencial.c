#include <math.h>
#include <stdio.h>


//Polinomio sobre el que vamos a trabajar.
#define RAIZ(x) (x - 4)

int main() {
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

  //Limite superior en el plano.
  xPositivo = 1000000;
  //Limite Inferior en el plano.
  xNegativo = -1000000;

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

  //Problema con puntos demasiado cercanos.
  } while(ea > 0.2 || (ea == 0.0 && i == 1));

  if (RAIZ(xRaiz) > -0.1 && RAIZ(xRaiz) < 0.1){
    //Mostramos las raices a medida que las encontramos en los rangos.
    printf("\nLa raiz encontrada para la funcion f(x) = x - 4, es: %f\n", xRaiz);
  }

  return 0;
}//FIN
