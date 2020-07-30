#include <chrono>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include "solucion.h"
#include <queue>
#include <fstream>
#include <cmath>

using namespace std;


// ============================================================================
// =========================== LECTURA DE FICHEROS ============================
// ============================================================================

void LeerCiudades(string &nombre, map<unsigned, pair<double, double>> &M) {
  ifstream datos;
  string s;
  pair<double, double> p;
  unsigned n, act;

  datos.open(nombre.c_str());
  if (datos.is_open()) {
    datos >> s; // Leo "dimension:"
    datos >> n;
    unsigned i = 0;
    while (i < n) {
      datos >> act >> p.first >> p.second;
      M[act] = p;
      i++;
    }

    datos.close();
  } else {
    cout << "Error de Lectura en " << nombre << endl;
  }
}

// ------------------------------------=

void LeerOrdenOptimo(string &nombre, vector<unsigned> &V) {
  ifstream datos;
  string s;
  pair<double, double> p;
  unsigned n, act;

  datos.open(nombre.c_str());
  if (datos.is_open()) {
    datos >> s; // Leo "dimension:"
    datos >> n;
    V.reserve(n);
    unsigned i = 0;
    while (i < n) {
      datos >> act;
      V.push_back(act);
      i++;
    }

    datos.close();
  } else {
    cout << "Error de Lectura en " << nombre << endl;
  }
}


// ------------------------------------------------------------------

Solucion BranchAndBound(vector<vector<double>> matriz_adyacencia, const double & cota_global, const vector<double> & arista_menor, const unsigned & n_ciudades,
  unsigned & tam_max_priority_queue, unsigned & num_poda, unsigned & num_nodos_ex)
{

	vector<unsigned> ciudades_restantes;
	priority_queue<Solucion> Q;
	Solucion n_e(n_ciudades), mejor_solucion(n_ciudades) ; //nodoe en expansion

	double CG=cota_global; // Cota Global
	double distancia_actual = 0;

	Q.push(n_e);

	while ( !Q.empty() && (Q.top().CotaLocal() < CG) ){
		n_e = Q.top();
    	num_nodos_ex++;
		Q.pop();
		ciudades_restantes = n_e.RestoCiudades();

		for (unsigned i = 0; i < ciudades_restantes.size(); ++i) {
			n_e.AniadirCiudad(ciudades_restantes[i], matriz_adyacencia);
			n_e.QuitarCiudadRestante(ciudades_restantes[i]);
			
			if ( n_e.EsSolucion(n_ciudades) ){
				distancia_actual = n_e.Evalua(matriz_adyacencia);
				if (distancia_actual < CG){ 
					CG = distancia_actual; 
					mejor_solucion = n_e; 
				}
			} 

			else{
        		n_e.calcularCotaLocal(arista_menor);

        		if (n_e.CotaLocal()<CG ){
					Q.push( n_e );

         			if(Q.size() > tam_max_priority_queue)
            			tam_max_priority_queue = Q.size();
        		}

        		else
          			num_poda++;
			}
			
			n_e.QuitarCiudad(matriz_adyacencia);
			n_e.AniadirCiudadRestante(ciudades_restantes[i]);
		}
	}

	return mejor_solucion;
}

double DistanciaEuclidea(pair<double, double> p1, pair<double, double> p2) {

  double modulo_x = (p1.first - p2.first) * (p1.first - p2.first);
  double modulo_y = (p1.second - p2.second) * (p1.second - p2.second);
  double distancia = sqrt(modulo_x + modulo_y);

  return distancia;
}

double DistanciaCircuito(map<unsigned, pair<double, double>> &m, vector<unsigned> &ordenado) {

  unsigned v_size = ordenado.size(), distancia_total = 0;

  for (auto it = ordenado.begin(); it != ordenado.end() - 1; ++it) {

    distancia_total += DistanciaEuclidea(m[(*it)], m[*(it + 1)]);
  }

  distancia_total += DistanciaEuclidea(m[ordenado[v_size - 1]], m[ordenado[0]]);

  return distancia_total;
}

double HeurVecinoMasCercano(map<unsigned, pair<double, double>> &m) {

  vector<unsigned> ruta;
  // De aqui vamos eliminando las ciudades que ya hemos recorrido
  map<unsigned, pair<double, double>> ciudades_restantes;
  double distancia;       // La distancia entre dos ciudades
  unsigned ciudad_elegida, ciudad_elegida_aux; // El n�mero de la Ciudad que elegimos
  unsigned distancia_menor; // La m�nima distancia posible entre dos ciudades

  ciudad_elegida = 1;
  ruta.push_back(1);
  ciudades_restantes = m;

  for (unsigned j = 1; j < m.size(); ++j) {

    ciudades_restantes.erase(ciudad_elegida);
    distancia_menor = DistanciaEuclidea(m[ciudad_elegida],
                                         ciudades_restantes.begin()->second);
    ciudad_elegida_aux = ciudades_restantes.begin()->first;

    for (auto ciu = ciudades_restantes.begin(); ciu != ciudades_restantes.end(); ++ciu) {
      distancia = DistanciaEuclidea(m[ciudad_elegida], ciu->second);

      if (distancia < distancia_menor) {
        distancia_menor = distancia;
        ciudad_elegida_aux = ciu->first;
      }
    }

    ciudad_elegida = ciudad_elegida_aux;
    ruta.push_back(ciudad_elegida);
   
  }
  
  
  return DistanciaCircuito(m, ruta);
}

void MatrizAdyacencia(map<unsigned, pair<double, double> > m,vector< vector<double> > & matriz_adyacencia){

	for(unsigned i=0; i<matriz_adyacencia.size(); i++) {
        for(unsigned j=0; j<matriz_adyacencia[i].size(); j++)
            if(i!=j) {
                matriz_adyacencia[i][j]=DistanciaEuclidea(m[i+1],m[j+1]);
            }

            else
              matriz_adyacencia[i][j] = 0;
	}
}

void MostrarMatriz(vector< vector<double> > & matriz){
	for(unsigned i=0; i<matriz.size(); i++) {
        for(unsigned j=0; j<matriz[i].size(); j++)
        	cout << matriz[i][j] << " ";
        cout << endl;
	}
}

vector<double> calcularMejorArco(const vector< vector<double> > & m) {
    vector<double> salida(m.size());

    double min;

    for(unsigned i = 0; i < m.size(); ++i){
      if(i == 0)
        min = m[i][1];

      else
        min = m[i][0];

      for(unsigned j = 0; j < m.size(); ++j){
        if(m[i][j] < min && m[i][j] != 0)
          min = m[i][j];

      }

      salida[i] = min;
    }
    return salida;
}

int main(int argc, char **argv){


	map<unsigned, pair<double, double>> M;
	vector<vector<double>> matriz_adyacencia;
  vector<double> mejores_arcos;
	
	string ruta_archivo_tsp = argv[1];

  LeerCiudades(ruta_archivo_tsp, M);

	matriz_adyacencia.resize(M.size());
	for(unsigned i=0; i<matriz_adyacencia.size(); i++)
        matriz_adyacencia[i].resize(M.size());

	MatrizAdyacencia(M, matriz_adyacencia);
	
	double cota_global = HeurVecinoMasCercano(M);
	
	// MostrarMatriz(matriz_adyacencia);

  mejores_arcos = calcularMejorArco(matriz_adyacencia);

  unsigned tam_max_priority_queue = 0;
  unsigned num_poda = 0;
  unsigned num_nodos_ex = 0;

	auto time_before = std::chrono::high_resolution_clock::now (); // {

	Solucion s = BranchAndBound(matriz_adyacencia, cota_global, mejores_arcos, M.size(), tam_max_priority_queue, num_poda, num_nodos_ex);

	auto time_after = std::chrono::high_resolution_clock::now (); // }

	auto total_time = std::chrono::duration_cast<std::chrono::duration<double>>(time_after - time_before).count();

  double distancia = DistanciaCircuito(M, s.getSolucion());

  cout << "El recorrido es el siguiente: " << endl;

  for(unsigned i = 0; i < s.getSolucion().size();++i)
    cout << s.getSolucion()[i] << endl;
  
  cout << endl;
  cout << "Con una distancia de " << distancia << endl;
  cout << "Tiempo necesitado: " << total_time << endl;
  cout << "Número de nodos expandidos: " << num_nodos_ex << endl;
  cout << "Tamaño máximo de la cola con prioridad: " << tam_max_priority_queue << endl;
  cout << "Número de veces que hemos podado: " << num_poda << endl;

	return 0;
}
