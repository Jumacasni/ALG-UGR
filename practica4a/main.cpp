#include <chrono>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include "solucion.h"

using namespace std;

unsigned soluciones_totales = 0;

/**
 * @brief Función backtracking que consigue la solución al problema
 * @param Sol: objeto Solucion donde escribiremos la solución encontrada
 * @param k: índice de la componete actual que estamos calculando
 */
void back_recursivo(Solucion & Sol, unsigned k)
{
	if (Sol.SolucionEncontrada()){
		if(soluciones_totales < 1)
			Sol.MostrarSolucion();
		++soluciones_totales;
	}
	else {
		Sol.IniciaComponente(k);
		Sol.SigValComp(k);
		while (!Sol.TodosGenerados(k)) {
			if (Sol.Factible(k)){
				back_recursivo(Sol, k+1);
				Sol.VueltaAtras(k);
			}
			Sol.SigValComp(k);
		}
	}
}


int main(int argc, char **argv){

	cout << endl;

	if(argc < 4){
		cout << "Use: " << argv[0] << " <tamanio_tablero> <x_inicial> <y_inicial>" << endl;
		exit(-1);
	}

	unsigned tam = atoi(argv[1]);
	unsigned x = atoi(argv[2]);
	unsigned y = atoi(argv[3]);

	Solucion s(tam,x,y);
	if(!s.ProblemaFactible() or x >= tam or y >= tam){
		std::cerr << "Problema no factible. El tamaño del tablero cuadrado debe ser mayor o igual que 5 y las posiciones iniciales tienen que ser menores que el tamaño del tablero." << endl << endl;
		exit(-1);
	}

	auto time_before = std::chrono::high_resolution_clock::now (); // {

	back_recursivo(s, 0);

	auto time_after = std::chrono::high_resolution_clock::now (); // }

	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(time_after - time_before).count();

	if(soluciones_totales > 0){
		cout << "Se han encontrado en total " << soluciones_totales << " soluciones en " << total_time << " segundos." << endl << endl;
	}
	else{
		cout << "No se han encontrado soluciones para la posición inicial (" << x << ", " << y << ")." << endl << endl;
	}

	return 0;
}
