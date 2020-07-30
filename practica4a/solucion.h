
#ifndef __SOLUCION_H__
#define __SOLUCION_H__

#include <vector>

using namespace std;


/** Tenemos 8 movimientos:

	k == 1 -> fil+1 y col+2
	k == 2 -> fil+2 y col+1
	k == 3 -> fil+2 y col-1
	k == 4 -> fil+1 y col-2     | |6| |7| |
	k == 5 -> fil-1 y col-2     |5| | | |8|
	k == 6 -> fil-2 y col-1     | | |I| | |
	k == 7 -> fil-2 y col+1     |4| | | |1|
	k == 8 -> fil-1 y col+2     | |3| |2| |
**/



class Solucion{

public:
	/**
	 * @brief constructor de Solucion
	 * @param tam_max: tamaño del tablero
	 * @param i: fila en la que empezamos
	 * @param j: columna en la que empezamos
	 */
	Solucion(const unsigned & tam_max, const unsigned & i, const unsigned & j);

	/**
	 * @brief comprueba las condiciones de factibilidad del ProblemaFactible
	 * @return True si es posible encontrar una solución al ProblemaFactible
	 */
	bool ProblemaFactible();

	/**
	 * @brief Función que comprueba el número de columnas de nuestra matriz
	 * @return Tamaño de la matriz (filas == columnas)
	 */
	unsigned size() const;

	/**
	 * @brief Inicializa una componente de la solución a 0
	 * @param k: índice de la componente a inicializar a 0
	 */
	void IniciaComponente(unsigned k);

	/**
	 * @brief Siguiente valor válido del dominio
	 * @param k: índice de la componente que queremos aumentar en uno
	 */
	void SigValComp(unsigned k);

	/**
	 * @brief Testea si quedan valores de S por generar
	 * @param k: índice de la componente que queremos comprobar
	 * @return True si están todos los valores generados
	 */
	bool TodosGenerados(unsigned k);

	/**
	 * @brief Muestra por pantalla el proceso realizado hasta alcanzar la solución. Permite quedarnos con la mejor solución
	 */
	void MostrarSolucion();

	/**
	 * @brief Comprueba si la solución actual cumple las restricciones
	 * @param k: índice de la componente que queremos comprobar
	 * @return True si es un movimiento factible
	 */
	bool Factible(unsigned k);

	/**
	* @brief Vuelve un movimiento atrás a la componente indicada
	* @param k: índice de la componente que queremos atrasar
	*/
	void VueltaAtras(unsigned k);

	/**
	 * @brief Comprueba que se ha pasado pro todo el tablero
	 * @return True si todas las componentes de la matriz M estan a 1
	 */
	bool SolucionEncontrada() const;


private:
	vector< vector<unsigned> > M;
	vector< unsigned > solucion;
	unsigned fil, col;

};

#endif // __SOLUCION_H__
