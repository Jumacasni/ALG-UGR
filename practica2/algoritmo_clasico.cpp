/**
   @file Algoritmo clásico para sumas parciales de elementos consecutivos
*/

   
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <chrono>

using namespace std;
using namespace std::chrono;



/* ************************************************************ */ 

static int suma_lims(int T[], int inicial, int final);

/**
   Implementación de las funciones
**/

inline int suma(int T[], int num_elem)
{
  if(num_elem == 0) return 0;
  if(num_elem == 1) return T[0];
  return suma_lims(T, 0, num_elem);
}


static int suma_lims(int T[], int inicial, int final)
{

  int i, j;
  int max = T[0] + T[1];
  int suma;

  for (i = inicial; i < final - 1; i++){
  	suma = T[i];
  	for(j = i+1; j < final; j++){
  		suma = suma + T[j];
  		if(suma > max)
  			max = suma;
  	}
  }

  return max;
}

double uniforme() //Genera un número uniformemente distribuido en el
                  //intervalo [0,1) a partir de uno de los generadores
                  //disponibles en C.
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

int main(int argc, char * argv[])
{

  if (argc != 2)
    {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
    }

  int n = atoi(argv[1]);

  int * T = new int[n];
  assert(T);

	srand(time(0));

	for (int j=0; j<n; j++) {
    	double u=uniforme();
    	T[j]= (int) (-51+102*u);
	}

  high_resolution_clock::time_point tantes, tdespues;
  duration<double> tiempo_transcurrido;

  tantes = high_resolution_clock::now();
  int max = suma(T, n);
  tdespues = high_resolution_clock::now();

  for (int j=0; j<n; j++) {cout << T[j] << " ";}
	cout << endl;

  tiempo_transcurrido  = duration_cast<duration<double> >(tdespues - tantes);
  cout << n << " " << tiempo_transcurrido.count() << endl;

  delete [] T;

  cout << "Resultado: " << max << endl;

  return 0;
}
 
