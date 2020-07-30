#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <stdio.h>

using namespace std;
using namespace std::chrono;

#define INF 999999999.0
#define DEBUG
bool OUTPUT = false;

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

// ============================================================================
// ================================== UTILES ==================================
// ============================================================================


double DistanciaEuclidea(pair<double, double> p1, pair<double, double> p2) {

  double modulo_x = (p1.first - p2.first) * (p1.first - p2.first);
  double modulo_y = (p1.second - p2.second) * (p1.second - p2.second);
  double distancia = sqrt(modulo_x + modulo_y);

  return distancia;
}

// ------------------------------------

double DistanciaCircuito(map<unsigned, pair<double, double>> &m, vector<unsigned> &ordenado) {

  unsigned v_size = ordenado.size(), distancia_total = 0;

  for (auto it = ordenado.begin(); it != ordenado.end() - 1; ++it) {

    distancia_total += DistanciaEuclidea(m[(*it)], m[*(it + 1)]);
  }

  distancia_total += DistanciaEuclidea(m[ordenado[v_size - 1]], m[ordenado[0]]);

  return distancia_total;
}

// ------------------------------------

double DistanciaCircuito(map<unsigned, pair<double, double>> &m, list<unsigned> &ordenado) {

  unsigned v_size = ordenado.size();
  double distancia_total = 0.0;
  auto end = ordenado.end();

  --end;

  for (auto it = ordenado.begin(); it != end; ++it) {
    auto aux = it;
    ++aux;
    distancia_total += DistanciaEuclidea(m[*(it)], m[*(aux)]);
  }

  distancia_total += DistanciaEuclidea(m[*(end)], m[*(ordenado.begin())]);

  return distancia_total;
}

// ------------------------------------

struct Ciudad {

  double x, y;
  unsigned indice;

  void Set(double x, double y, unsigned indice) {
    this->x = x;
    this->y = y;
    this->indice = indice;
  }

};

// ------------------------------------

struct InsercionCiudad{

  double dist_recorrido;
  list<unsigned>::iterator posicion_lista;
  map<unsigned, pair<double, double>>::iterator indice_mapa;

  void Set(double dist_recorrido, list<unsigned>::iterator posicion_lista, map<unsigned, pair<double, double>>::iterator indice_mapa){
    this->dist_recorrido = dist_recorrido;
    this->posicion_lista = posicion_lista;
    this->indice_mapa = indice_mapa;
  }

};

// ============================================================================
// =============================== HEURISTICAS ================================
// ============================================================================

void HeurVecinoMasCercano(map<unsigned, pair<double, double>> &m, vector<unsigned> &ruta) {

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

    for (auto ciu = ciudades_restantes.begin(); ciu != ciudades_restantes.end();
         ++ciu) {
      distancia = DistanciaEuclidea(m[ciudad_elegida], ciu->second);

      if (distancia < distancia_menor) {
        distancia_menor = distancia;
        ciudad_elegida_aux = ciu->first;
      }
    }

    ciudad_elegida = ciudad_elegida_aux;
    ruta.push_back(ciudad_elegida);
  }
}

// ------------------------------------
// ------------------------------------

void HeurInsercion(map<unsigned, pair<double, double>> &m, vector<unsigned> &ruta) {

  // De aqui vamos eliminando las ciudades que ya hemos recorrido
  map<unsigned, pair<double, double>> ciudades_restantes(m);
  list<unsigned> lista_ruta;
  double distancia_total = 0;
  Ciudad mas_izq, mas_der, mas_nor;


  mas_izq.Set(INF, 0, 0);
  mas_der.Set(0, 0, 0);
  mas_nor.Set(0, 0, 0);


  // Obtengo las ciudades más a la izquierda (mas_izq), más a la derecha
  // (mas_der) y más al norte (mas_nor)
  for (auto &it : m) {

    unsigned indice_actual = it.first;
    double x_actual = it.second.first;
    double y_actual = it.second.second;

    if (x_actual < mas_izq.x)
      mas_izq.Set(x_actual, 0, indice_actual);

    if (x_actual > mas_der.x)
      mas_der.Set(x_actual, 0, indice_actual);

    if (y_actual > mas_nor.y)
      mas_nor.Set(0, y_actual, indice_actual);
  }


  // Inicializo mi lista de ciudades con las 3 obtenidas anteriormente
  unsigned lista_inicial [] = {mas_izq.indice, mas_der.indice, mas_nor.indice};
  lista_ruta.insert(lista_ruta.end(), lista_inicial, lista_inicial + 3);


  // Borro de las ciudades_restantes las ya utilizadas
  for(unsigned i = 0; i < 3; ++i)
    ciudades_restantes.erase(lista_inicial[i]);


  InsercionCiudad mejor_ciudad;


  // Hasta que me quede sin ciudades en el mapa, obtengo la mejor Ciudad para
  // insertar en mi ruta actual
  while(not ciudades_restantes.empty()){


    // Para cada Ciudad, calculo la distancia de ruta situandola entre cada 2
    // ciudades de mi ruta actual
    for(auto it_ciudad = ciudades_restantes.begin(); it_ciudad != ciudades_restantes.end(); ++it_ciudad){


      mejor_ciudad.Set(INF, lista_ruta.begin(), m.begin());
      unsigned tam_l_orden = lista_ruta.size();
      auto it_l_orden = lista_ruta.begin();


      // calculo la la posicion en que la distancia es menor y guardo la
      // posición en la lista
      for(unsigned i = 0; i < tam_l_orden; ++i, ++it_l_orden){

          lista_ruta.insert(it_l_orden, it_ciudad->first);

          double distancia_actual = DistanciaCircuito(m, lista_ruta);


          // Si es mejor que la distancia anterior, actualiza
          // la distancia de recorrido, la posición actual en la lista
          if(distancia_actual < mejor_ciudad.dist_recorrido)
            mejor_ciudad.Set(distancia_actual, it_l_orden, it_ciudad);


          auto aux = it_l_orden;
          --aux;
          lista_ruta.erase(aux);


      } // for lista_ruta


    } // for ciudades_restantes

    // Inserto en la lista ordenada la Ciudad que menor distancia ha dado
    // la posición mejor posición
    lista_ruta.insert(mejor_ciudad.posicion_lista, mejor_ciudad.indice_mapa->first);

    ciudades_restantes.erase(mejor_ciudad.indice_mapa);

    #if defined DEBUG
        printf ("%c[2K", 0x1B);
        cout  << "\r[DEBUG]\t"
              << "Ciudades restantes: " << ciudades_restantes.size()
              << "\tLista de ciudades: " << lista_ruta.size() << flush;
    #endif
  }

  ruta.resize(lista_ruta.size());

  std::copy(lista_ruta.begin(), lista_ruta.end(), ruta.begin());

  #if defined DEBUG
    printf ("%c[2K\r", 0x1B);
    cout << flush;
  #endif
}

// ------------------------------------
// ------------------------------------

vector<unsigned> DosOptSwap( const int& i, const int& k, vector<unsigned> &ruta ){

  unsigned v_size = ruta.size();
  vector<unsigned> nuevo(v_size, 0);

  // 1. take route[0] to route[i-1] and add them in order to new_route
  for ( int c = 0; c < i; ++c ){

      nuevo[c] = ruta[c];
  }

  // 2. take route[i] to route[k] and add them in reverse order to new_route
  int dec = 0;
  for ( int c = i; c <= k; ++c ){

    nuevo[c] = ruta[k - dec];
    dec++;
  }

  // 3. take route[k+1] to end and add them in order to new_route
  for ( int c = k + 1; c < v_size; ++c ){

      nuevo[c] = ruta[c];
  }

  return nuevo;
}

// Do all 2-opt combinations
void HeurDosOpt(map<unsigned, pair<double, double>> &m, vector<unsigned> &ruta){
    // Get tour size
    int size = m.size();
    vector<unsigned> nueva_ruta;

    // repeat until no improvement is made
    int mejora = 0;

    while ( mejora < 20 ) {

        double mejor_distancia = DistanciaCircuito(m, ruta);

        for ( int i = 0; i < size - 1; i++ ){

            for ( int k = i + 1; k < size; k++){

                nueva_ruta = DosOptSwap( i, k, ruta);

                double distancia_actual = DistanciaCircuito(m, nueva_ruta);

                if ( distancia_actual < mejor_distancia ) {
                    // Improvement found so reset
                    mejora = 0;
                    ruta = nueva_ruta;
                    mejor_distancia = distancia_actual;
                }
            }
        }

        mejora++;
    }
}

// ============================================================================
// ================================== MAIN ====================================
// ============================================================================

void PrintHelp (char *prog) {
  fprintf (stderr,
    "\tUso:\t%s <opciones>\n"
    "\n"
    "-f <ruta_fichero>\n"
    "\tRuta al fichero tsp.\n"
    "\t\"datosTSP/<nombre_fichero>.tsp\"\n"
    "\n"
    "-e <heurística>\n"
    "\tUna de las tres heurísticas disponibles:\n"
    "\t\"0\" - Solución óptima del problema\n"
    "\t\"1\" - Vecino más cercano (POR DEFECTO)\n"
    "\t\"2\" - Inserción\n"
    "\t\"3\" - Dos opt\n"
    "\n"
    "-o\n"
    "\tImprimir a un fichero las ciudades para pintarlas con gnuplot.\n"
    "\t\"<nombre_fichero>.out.tsp\"\n",
    prog);
  exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------

int main(int argc, char **argv) {

  if (argc == 1 or string(argv[1]) == "--help" or string(argv[1]) == "-h") {
    PrintHelp(argv[0]);
  }

  map<unsigned, pair<double, double>> M;
  vector<unsigned> V, orden_ciudades;
  unsigned heuristica_elegida;
  double distancia = 0;
  string ruta_archivo_tsp, nombre_archivo, ruta_archivo_opt_tour, nombre_heuristica;


  // f + fichero, e + numero, o true
  char c;
  while ((c = getopt (argc, argv, "f:e:ot")) != -1) {
    switch (c) {
      case 'f': {
        ruta_archivo_tsp = string (optarg);
        ruta_archivo_opt_tour = ruta_archivo_tsp.substr(0, ruta_archivo_tsp.rfind(".")) + string(".opt.tour");
        unsigned start = ruta_archivo_tsp.rfind ('/') + 1;
        unsigned end = ruta_archivo_tsp.find('.');
        nombre_archivo = ruta_archivo_tsp.substr(start, end - start);
      } break;
      case 'e': {
        if (isxdigit (optarg[0]))
          heuristica_elegida = stoi (optarg);
        else
          PrintHelp(argv[0]);
      } break;
      case 'o': {
        OUTPUT = true;
      } break;
      case '?': {
        if (optopt == 'f')
          fprintf (stderr, "La opción -%c requiere la ruta al fichero tsp.\n", optopt);
        else if (optopt == 'e')
          fprintf (stderr, "La opción -%c requiere una de las heurísticas.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option '-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        PrintHelp (argv[0]);
      } break;
    }
  }

  LeerCiudades(ruta_archivo_tsp, M);

  LeerOrdenOptimo(ruta_archivo_opt_tour, V);


  cout << endl;

  auto time_before = high_resolution_clock::now (); // {

  switch (heuristica_elegida) {
    case 0:{
      nombre_heuristica = "optimo";
      cout << "Obteniendo el orden óptimo..." << endl;
      orden_ciudades.resize(V.size());
      std::copy(V.begin(), V.end(), orden_ciudades.begin());
      distancia = DistanciaCircuito(M, orden_ciudades);
    } break;
    case 1:{
      nombre_heuristica = "vecino_cercano";
      cout << "Obteniendo el orden con la heurística del vecino más cercano..." << endl;
      HeurVecinoMasCercano(M, orden_ciudades);
      distancia = DistanciaCircuito(M, orden_ciudades);
    } break;
    case 2:{
      nombre_heuristica = "insercion";
      cout << "Obteniendo el orden con la heurística de inserción..." << endl;
      HeurInsercion(M, orden_ciudades);
      distancia = DistanciaCircuito(M, orden_ciudades);
    } break;
    case 3:{
      nombre_heuristica = "dos_opt";
      cout << "Obteniendo el orden con la heurística de dos opt..." << endl;
      HeurVecinoMasCercano(M, orden_ciudades);
      HeurDosOpt(M, orden_ciudades);
      distancia = DistanciaCircuito(M, orden_ciudades);
    } break;
  }

  auto time_after = high_resolution_clock::now (); // }

  cout << endl;

  auto total_time = duration_cast<duration<double>>(time_after - time_before);
  cout  << "Finalizada la heurística en " << total_time.count()
        << " segundos." << endl
        << "Distancia mínima conseguida con la heurística de "
        << distancia << " unidades." << endl;


  cout << endl;

  std::streambuf * buf;
  ofstream of;

  if(OUTPUT){
    string archivo_salida = string("salidas/") + nombre_archivo + string(".") + nombre_heuristica + string(".salida.tsp");
    of.open(archivo_salida.c_str());
    buf = of.rdbuf();
  }
  else {
    buf = std::cout.rdbuf();
  }


  ostream fout(buf);

  if(!fout){
    cerr << "Error al abrir el archivo de salida." << endl;
    exit(EXIT_FAILURE);
  }

  unsigned tam_salida = orden_ciudades.size();
  fout << "Numero de ciudades " << tam_salida++ << endl;

  // Añado la primera ciudad para que con gnuplot haga un ciclo
  orden_ciudades.push_back(orden_ciudades[0]);

  for(unsigned i = 0; i < tam_salida; ++i)
    fout << orden_ciudades[i] << "\t" << M[orden_ciudades[i]].first << "\t" << M[orden_ciudades[i]].second << endl;

  return 0;
}
