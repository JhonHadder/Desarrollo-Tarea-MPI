#define N 10000
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    //Valores necesarios para el calculo de la integral.
    //Las letras son las generalmente utilizadas en el calculo integral.
    double sumaInt, xi, a, b, w;

    //Intervalo o rango de valores en el que realizaremos la Integral
    a = 0.0; b = 1.0;

    //Funcion el area del rectangulo que como sabemos nos permite:
    //Por medio de rectangulos encontrar el area bajo la curva de una funcion.
    w = (b-a)/N;
    sumaInt = 0.0;
    for (int i = 1; i <= N; i++) {
      xi = a + (i-1)*w;
      //Sumas de la iteracion de la funcion integrada.
      sumaInt = sumaInt + (w * (xi * xi * xi));
    }

    //Mostramos el valor calculado de la integral en el intervalo propuesto.
    printf("Valor retornado por la Integral = %f\n",sumaInt);
    return 0;
}
