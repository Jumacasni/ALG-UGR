#include <iostream>
#include "solucion.h"

double distanciaCiudades(unsigned ciudad1,unsigned ciudad2,const vector< vector<double> > & m) {
    return m[ciudad1-1][ciudad2-1];
}

Solucion::Solucion(const unsigned & tam_max){
	pos_e = estimador = distancia = 0;
	x.push_back(1);
	ciudades_restantes.resize(tam_max-1, 0);
	
	unsigned i = 2;
	
	for(auto &it: ciudades_restantes){
		it = i;
		++i;
	}
}

double Solucion::CotaLocal() const{
	return estimador;
}

double Solucion::Evalua(vector<vector<double>> matriz){
	distancia+=distanciaCiudades(x.back(),1,matriz);
	return distancia;
}

bool Solucion::EsSolucion(unsigned tam) const{
	return x.size() == tam;
}

vector<unsigned> Solucion::RestoCiudades() const{
	vector<unsigned> aux;
	
	for(auto &it: ciudades_restantes)
		aux.push_back(it);

	return aux;
}

void Solucion::AniadirCiudad(unsigned k,const vector< vector<double> > &m){
	distancia += distanciaCiudades(x.back(),k,m);
	x.push_back(k);
}

void Solucion::QuitarCiudadRestante(unsigned k) {
        for (auto it = ciudades_restantes.begin(); it != ciudades_restantes.end(); ++it) {
            if(*it==k) {
                ciudades_restantes.erase(it);
                break;
            }
        }
}

void Solucion::QuitarCiudad(const vector< vector<double> > & matriz){
	distancia -= distanciaCiudades(x[x.size()-2],x.back(),matriz);
	x.pop_back();
}

void Solucion::AniadirCiudadRestante(unsigned k) {
        ciudades_restantes.push_back(k);
}

void Solucion::calcularCotaLocal(const vector<double> & mejores_arcos){
	estimador = distancia;

	for(auto it = ciudades_restantes.begin(); it != ciudades_restantes.end(); ++it)
		estimador += mejores_arcos[(*it)-1];
}

double Solucion::getDistancia() const{
	return distancia;
}

vector<unsigned> & Solucion::getSolucion(){
	return x;
}