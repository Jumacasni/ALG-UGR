/**
   @file C�lculo de la sucesi�n de Fibonacci
*/

   
#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;
/**
   @brief Calcula el t�rmino n-�simo de la sucesi�n de Fibonacci.

   @param n: n�mero de orden del t�rmino buscado. n >= 1.

   @return: t�rmino n-�simo de la sucesi�n de Fibonacci.
*/
int fibo(int n)
{
  if (n < 2)
    return 1;
  else
    return fibo(n-1) + fibo(n-2);
}


int main(int argc, char * argv[])
{
  if (argc != 2)
  {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
  }

  int n = atoi(argv[1]);
  int f;

  high_resolution_clock::time_point tantes, tdespues;
  duration<double> tiempo_transcurrido;

  tantes = high_resolution_clock::now();
  f = fibo(n);
  tdespues = high_resolution_clock::now();

  tiempo_transcurrido  = duration_cast<duration<double> >(tdespues - tantes);
  cout << n << " " << tiempo_transcurrido.count() << endl;

  return 0;
}
 
