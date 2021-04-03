#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <time.h>
#include <sys/time.h>
#include "math.h"
#include "random.h"

using namespace std;

/**
  *@brief Baraja una lista de tipo entero.
  *@param lista La lista que queremos desordenar aleatoriamente.
  */
void barajar(list<int> &lista){
  vector <int> listovec(lista.begin(), lista.end());
  random_shuffle(listovec.begin(), listovec.end());
  copy(listovec.begin(), listovec.end(), lista.begin());
}


/**
  *@brief Comprueba si j se encuentra dentro del vector pasado por parametro.
  *@param j El indice a comprobar dentro del vector
  *@param vecSolucion Vector sobre el que comprobar la existencia de un elemento j.
  */
bool estaDentro(int j, vector<int> vecSolucion){

	for(auto i=0;i<vecSolucion.size();i++){
		if(j==vecSolucion[i]) return true;
	}
	return false;
}

/**
  *@brief Comprueba si j se encuentra dentro de la lista pasada por parametro.
  *@param j El indice a comprobar dentro de la lista
  *@param vecSolucion La lista sobre la que comprobar la existencia de un elemento j.
  */
bool estaDentro(int j, list<int> vecSolucion){

	for(auto it=vecSolucion.begin();it!=vecSolucion.end();it++){
		if((*it)==j) return true;
	}
	return false;
}


/**
  *@brief Ordena en funcion de los aportes.
  *@param listaSolucion Lista de nodos solucion desordenados.
  *@param aportes Vector de aportes de cada nodo.
  *@param listaSolOrdenada Lista de nodos solucion en el que guardaremos los nodos ordenados.
  *@param aportesOrdenado Vector de aportes de cada nodo en el que guardaremos los aportes ordenados.
  */
void ordenacionPorAporte(list<int> &listaSolucion, vector<double> &aportes, list<int> &listaSolOrdenada, vector<double> &aportesOrdenado){

    auto itSolucion = listaSolucion.begin(), indiceMinimo = listaSolucion.begin();
    auto valorMinimo = aportes.begin();
                        
    //BUSCAREMOS SIEMPRE EL MENOR APORTE PARA INTRODUCIRLO AL FINAL DE LA LISTA ORDENADA
    for(auto itAporte = aportes.begin(); itAporte < aportes.end(); itAporte++){
        if(*itAporte < *valorMinimo){
                valorMinimo = itAporte;
                indiceMinimo = itSolucion;
        }
        itSolucion++;
    }
                        
    aportesOrdenado.push_back((*valorMinimo));
    listaSolOrdenada.push_back((*indiceMinimo));
    aportes.erase(valorMinimo);
    listaSolucion.erase(indiceMinimo);

}


/**
  *@brief Calcula el coste total de la lista solucion final.
  *@param listaSolucion Lista de nodos solucion.
  *@param vecDistancias Matriz de todos los aportes de cada nodo respecto a los demas.
  */
double calculoCosteFinal(list<int> listaSolucion, vector<vector<double>> vecDistancias){

    auto costeFinal = 0.0;

  for(auto i = listaSolucion.begin(); i != listaSolucion.end(); i++){
    for(auto j = i; j != listaSolucion.end(); j++){
      costeFinal += vecDistancias[*i][*j];
    }
  }
  return costeFinal;
}



int main(int argc,char *argv[]){

	int n, m, k, l, contador = 0, LIMITE=100000;
	double distancia;


	if(argc!=3){
    		printf("Datos mal introducidos.\n");
    		exit(1); 
	}

	string file_name = argv[1];

	int Semilla = stoi(argv[2]);

	ifstream File;
	File.open("../datos/"+file_name);	

	if(File.fail()){
		cout<<"Error al abrir el archivo, por favor, compruebe el nombre del fichero de entrada..."<<endl;
		exit(1);
	}else{

		File >> n >> m;	

	Set_random(Semilla);

        auto numIteraciones = n*(n-1)/2;
        auto hastaRellenar = 0;

	vector<vector<double>> vecDistancias (n);

        for(auto i = 0; i < n; i++)
            vecDistancias[i].resize(n);

        for(auto i = 0; i<numIteraciones; i++){
            File >> k >> l >> distancia;
            vecDistancias[k][l]=distancia;
            vecDistancias[l][k]=distancia;
        }

		File.close();

	cout<<"-------------------------------------------"<<endl;
	cout<<"Tamanio del problema: "<<n<<"\n"<<"Tamanio del espacio de soluciones: "<<m<<endl;
	cout<<"-------------------------------------------"<<endl;


	//-----------------------------------//
	//***     Enfoque Primer Mejor    ***//
	//-----------------------------------//



	list<int> listaSolucion, listaCandidatos;

        auto Semilla2 = Get_random();

        cout<<"---> Usando semilla: "<<Semilla2<<endl;

        // RELLENAR VECTOR DE CANDIDATOS
        for(auto i = 0; i < n; i++){
            listaCandidatos.push_back(i);
        }

	// CREAR UNA PRIMERA SOLUCION ALEATORIA (VARIACION EN FUNCION DE SEMILLA GENERADA)
	for(auto i = 0; hastaRellenar < m; i++){            
        	auto iRandom = Randint(-1,n-1);
        	if(!estaDentro(iRandom,listaSolucion)){ 
                    listaSolucion.push_back(iRandom);
                    listaCandidatos.remove(iRandom); hastaRellenar++;
                }

        }

        auto cambio = 0;
        auto parada = false;

        auto t0=clock();
            
        while((cambio < LIMITE) && !parada){
                
            vector<double> aportes(m);
            auto i = 0, j=0;

            // FACTORIZACION DEL APORTE DE CADA NODO DE LA SOLUCION
            for(auto it = listaSolucion.begin(); it != listaSolucion.end(); it++, i++){
                j = i;
                for(auto jt = it; jt != listaSolucion.end(); jt++, j++){
                    aportes[i] += vecDistancias[*it][*jt];
                    aportes[j] += vecDistancias[*it][*jt];
                }
            }
                    
            // ORDENAREMOS EN ESTOS VECTORES
            vector<double> aportesOrdenado;
            list<int> listaSolOrdenada;
               
	    // ORDENAR MIENTRAS NO HAYAMOS VACIADO EL VECTOR DE APORTES
            while(!aportes.empty()){
                ordenacionPorAporte(listaSolucion, aportes, listaSolOrdenada, aportesOrdenado);
            }

            auto iter = aportesOrdenado.begin();
            bool mejora = false;

	    // BUSQUEDA DEL MEJOR VECINO PARA INTERCAMBIAR
            while((iter != aportesOrdenado.end()) && !mejora){

                auto costeMinimo = *iter;
                auto indiceMinimo = listaSolOrdenada.front();
                listaSolOrdenada.pop_front();

                // DESORDENAMOS LISTA DE CANDIDATOS INCREMENTANDO ALEATORIEDAD
                barajar(listaCandidatos);
                
                for(auto iterator = listaCandidatos.begin(); iterator!=listaCandidatos.end() && !mejora; iterator++){
                    
                    auto costeMejor = 0.0;

                    for(auto i = listaSolOrdenada.begin(); i != listaSolOrdenada.end(); i++){
                        costeMejor += vecDistancias[*iterator][*i];
                    }
                    
                    if(costeMejor > costeMinimo){
                        mejora = true;
                        listaSolOrdenada.push_back(*iterator);
                        listaCandidatos.remove(*iterator);
                        listaCandidatos.push_back(indiceMinimo);
                    }
                }

                // SI NO ENCONTRAMOS VECINO QUE MEJORE EL COSTE, VOLVEMOS A INTRODUCIR ESE ELEMENTO
		// Y PASAMOS A COMPROBAR EL SIGUIENTE
                if(!mejora){
                    listaSolOrdenada.push_back(indiceMinimo);
                    iter++;
                }
            }
            // SI NO ENCONTRAMOS VECINO MEJOR Y HEMOS RECORRIDO TODA LA LISTA
	    // DE CANDIDATOS, FINALIZAMOS.
            if(!mejora){
                parada = true;
            }
            listaSolucion = listaSolOrdenada;
            cambio++;
        }

        auto costeFinal = calculoCosteFinal(listaSolucion, vecDistancias);
            
        auto t1=clock();
        
	auto time = (double(t1-t0)/CLOCKS_PER_SEC);

	cout << "Tiempo de ejecucion Algoritmo Busqueda Local Primer Mejor: " << time << endl;
	cout<<"Coste Total Algoritmo Busqueda Local Primer Mejor: "<<costeFinal<<endl;




	//-----------------------------------//
	//***        Enfoque Greedy       ***//
	//-----------------------------------//


	vector<int> vecSolucion;

	auto valor=0.0, anterior=0.0;
	auto iBuena=0;

	auto tg0=clock();

	for(auto i=0;i<n;i++){
		for(auto j=0;j<n;j++){
			valor+=vecDistancias[i][j];
		}
		if(valor>=anterior){
			anterior=valor;
			iBuena = i;
		}
		valor=0.0;
	}
	
	vecSolucion.push_back(iBuena);

		while(contador<m){
				
			//CALCULAR DISTANCIAS ACUMULADAS ATENDIENDO AL SELECCIONADO

			auto costeAcumulado = 0.0;
			auto costeFinal = 0.0;
			auto iFinal = 0;

				for(auto i=0;i<n;i++){
					for(auto j=0; j<n;j++){
						if(estaDentro(j,vecSolucion)){
							costeAcumulado+=vecDistancias[i][j];					
						}
					}
					if(costeAcumulado>costeFinal && !(estaDentro(i,vecSolucion))){
						iFinal = i;
						costeFinal = costeAcumulado;
	
					}
					costeAcumulado=0.0;
				}
				vecSolucion.push_back(iFinal);

				contador++;

		}

	auto tg1=clock();

	auto timeG = (double(tg1-tg0)/CLOCKS_PER_SEC);

	auto costeTotalG=0.0;
		
	for(auto i=0; i<m-1;i++){
		for(auto j=i+1;j<m;j++){
			costeTotalG+=vecDistancias[vecSolucion[i]][vecSolucion[j]];					
		}	
	}
		
	cout << "Tiempo de ejecucion Algoritmo Greedy: " << timeG << endl;
	cout<<"Coste Total Algoritmo Greedy: "<<costeTotalG<<endl;

        
    }

	return 0;

}
