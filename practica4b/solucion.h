
#ifndef __SOLUCION_H__
#define __SOLUCION_H__

#include <vector>
#include <map>
#include <list>

using namespace std;

class Solucion{

public:

	Solucion(const unsigned & tam_max);

	double CotaLocal() const;

	double Evalua(vector<vector<double>> matriz);

	bool EsSolucion(unsigned tam) const;

	vector<unsigned> RestoCiudades() const;

	void AniadirCiudad(unsigned k,const vector< vector<double> > &m);
	
	void QuitarCiudadRestante(unsigned k);
	
	void QuitarCiudad(const vector< vector<double> > & matriz);
	
	void AniadirCiudadRestante(unsigned k);
	
	void calcularCotaLocal(const vector<double> & mejores_arcos);

	double getDistancia() const;

	vector<unsigned> & getSolucion();

	bool operator<(const Solucion & s) const{
		return estimador < s.estimador;
	}

private:
	vector< unsigned > x; // Almacenamos la solucion
	unsigned pos_e; // Posicion de la ultima decision en X
	double distancia;
	double estimador; // Valor estimado para el nodo X
	list<unsigned> ciudades_restantes;
};

#endif // __SOLUCION_H__
