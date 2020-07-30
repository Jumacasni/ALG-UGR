#include <iostream>
#include "solucion.h"

Solucion::Solucion(const unsigned & tam_max, const unsigned & i, const unsigned & j){
	M.resize(tam_max);
	solucion.resize(tam_max*tam_max);

	for(unsigned i = 0; i < tam_max; ++i)
		M[i].resize(tam_max);

	for(unsigned i = 0; i < tam_max; ++i)
		for(unsigned j = 0; j < tam_max; ++j)
			M[i][j] = 0;

	for(unsigned i = 0; i < tam_max*tam_max; ++i)
		solucion[i] = 0;

	fil = i;
	col = j;
	M[fil][col] = 1;
}


bool Solucion::ProblemaFactible(){
	return (M.size() >= 5);
}


unsigned Solucion::size() const{
	return (unsigned) M.size();
}


void Solucion::IniciaComponente(unsigned k){
	solucion[k] = 0;
}


void Solucion::SigValComp(unsigned k){
	solucion[k]++;
}


bool Solucion::TodosGenerados(unsigned k){
	return solucion[k] == 9;
}


void Solucion::MostrarSolucion(){

	cout << "Solución: " << endl;

	for(auto &it : solucion)
		cout << "Accion " << it << "; ";

	cout << endl << endl;

}


bool Solucion::Factible(unsigned k){
	if(solucion[k] == 1){
		if(fil+1 >= 0 && fil+1 < M.size() && col+2 >= 0 && col+2 < M.size() && M[fil+1][col+2] == 0){
			fil+=1;
			col+=2;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 2){
		if(fil+2 >= 0 && fil+2 < M.size() && col+1 >= 0 && col+1 < M.size() && M[fil+2][col+1] == 0){
			fil+=2;
			col+=1;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 3){
		if(fil+2 >= 0 && fil+2 < M.size() && col-1 >= 0 && col-1 < M.size() && M[fil+2][col-1] == 0){
			fil+=2;
			col-=1;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 4){
		if(fil+1 >= 0 && fil+1 < M.size() && col-2 >= 0 && col-2 < M.size() && M[fil+1][col-2] == 0){
			fil+=1;
			col-=2;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 5){
		if(fil-1 >= 0 && fil-1 < M.size() && col-2 >= 0 && col-2 < M.size() && M[fil-1][col-2] == 0){
			fil-=1;
			col-=2;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 6){
		if(fil-2 >= 0 && fil-2 < M.size() && col-1 >= 0 && col-1 < M.size() && M[fil-2][col-1] == 0){
			fil-=2;
			col-=1;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 7){
		if(fil-2 >= 0 && fil-2 < M.size() && col+1 >= 0 && col+1 < M.size() && M[fil-2][col+1] == 0){
			fil-=2;
			col+=1;
			M[fil][col] = 1;
			return true;
		}
	}

	else if(solucion[k] == 8){
		if(fil-1 >= 0 && fil-1 < M.size() && col+2 >= 0 && col+2 < M.size() && M[fil-1][col+2] == 0){
			fil-=1;
			col+=2;
			M[fil][col] = 1;
			return true;
		}
	}

	return false;
}


void Solucion::VueltaAtras(unsigned k){
	if(k<solucion.size()-1){
		solucion[k+1] = 0;
	}

	if(solucion[k] == 1){
		M[fil][col] = 0;
		fil-=1;
		col-=2;
	}

	else if(solucion[k] == 2){
		M[fil][col] = 0;
		fil-=2;
		col-=1;
	}

	else if(solucion[k] == 3){
		M[fil][col] = 0;
		fil-=2;
		col+=1;
	}

	else if(solucion[k] == 4){
		M[fil][col] = 0;
		fil-=1;
		col+=2;
	}

	else if(solucion[k] == 5){
		M[fil][col] = 0;
		fil+=1;
		col+=2;
	}

	else if(solucion[k] == 6){
		M[fil][col] = 0;
		fil+=2;
		col+=1;
	}

	else if(solucion[k] == 7){
		M[fil][col] = 0;
		fil+=2;
		col-=1;
	}

	else if(solucion[k] == 8){
		M[fil][col] = 0;
		fil+=1;
		col-=2;
	}
}


bool Solucion::SolucionEncontrada() const{
	for(unsigned i = 0; i < M.size(); ++i){
		for(unsigned j = 0; j < M.size(); ++j){
			if(M[i][j] == 0)
				return false;
		}
	}

	return true;
}
