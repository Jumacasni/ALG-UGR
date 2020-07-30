/**
   @file Algoritmo divide y vencerás para la sumas parciales de elementos consecutivos
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

const int UMBRAL_MS = 3;

inline int suma(int T[], int num_elem)
{
	if(num_elem < 2) return 0;
  return suma_lims(T, 0, num_elem-1);
}

inline int maximo(int a, int b){
	if (a >= b)
		return a;

	return b;
}

static int suma_lims(int T[], int inicial, int final)
{
	int max;

	if(final == inicial){
		max = T[inicial];
	}

	else{
		int mitad = (inicial + final) / 2;
		int izq = suma_lims(T, inicial, mitad);
		int der = suma_lims(T, mitad+1, final);
	
		int max_izq = T[mitad];
		int max_der = T[mitad+1];
		int suma = 0;

		for(int i = mitad; i >= inicial; i--){
			suma = suma + T[i];
			if(suma > max_izq)
				max_izq = suma;
		}

		suma = 0;
		for(int i = mitad+1; i <= final; i++){
			suma = suma + T[i];
			if(suma > max_der)
				max_der = suma;
		}

		max = maximo(izq,der);
		max = maximo(max, max_izq+max_der);
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
 
