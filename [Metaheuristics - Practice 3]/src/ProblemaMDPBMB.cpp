#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "math.h"
#include <algorithm>
#include <string>
#include <list>
#include <time.h>
#include <sys/time.h>
#include "random.h"

using namespace std;


double calculoCosteFinal(list<int> listaSolucion, vector<vector<double>> vecDistancias){

    auto costeFinal = 0.0;

  for(list<int>::iterator i = listaSolucion.begin(); i != listaSolucion.end(); i++){
    for(list<int>::iterator j = i; j != listaSolucion.end(); j++){
      costeFinal += vecDistancias[*i][*j];
    }
  }
  return costeFinal;
}

bool estaDentro(int j, list<int> listaSolucion){
	list<int>::iterator it=listaSolucion.begin();
    
    for(auto it=listaSolucion.begin();it!=listaSolucion.end(); ++it){
        if(*it==j){
            return true;
        }
    }

    return false;

}

list<int> intAleatorio(list<int> listaSol, int n){

    auto solRand = Randint(-1,n-1);
    auto candRand = Randint(-1,n-1);
    auto indice = 0, cont = 0;

    list<int> vecino;

    while (!estaDentro(solRand, listaSol) || estaDentro(candRand, listaSol)){
        (!estaDentro(solRand, listaSol) ? solRand = Randint(-1,n-1) : candRand = Randint(-1,n-1));
    }

    for(auto it=listaSol.begin(); it!=listaSol.end(); ++it, cont++){
        if(*it==solRand)
            indice = cont;
    }
    
    auto it=listaSol.begin();
    advance(it, indice);

    *it = candRand;
    vecino = listaSol;

    return vecino;

}

void barajar(list<int> &lista){
  vector <int> listovec(lista.begin(), lista.end());
  random_shuffle(listovec.begin(), listovec.end());
  copy(listovec.begin(), listovec.end(), lista.begin());
}

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

void BusquedaLocal(vector<vector<double>> vecDistancias, int m, int LIMITE, list<int> &listaSolucion, list<int> &listaCandidatos){

    auto cambio = 0;
    auto parada = false;

    auto t0=clock();

    //400 iteraciones BL
          
    while((cambio < LIMITE) && !parada){
                
        vector<double> aportes(m);
        auto i = 0, j=0;

        // FACTORIZACION DEL APORTE DE CADA NODO DE LA SOLUCION
        for(list<int>::iterator it = listaSolucion.begin(); it != listaSolucion.end(); it++, i++){
            j = i;
            for(list<int>::iterator jt = it; jt != listaSolucion.end(); jt++, j++){
                aportes[i] += vecDistancias[*it][*jt];
                aportes[j] += vecDistancias[*it][*jt];
            }
        }
                    
        //ORDENAMOS EN ESTOS VECTORES
        vector<double> aportesOrdenado;
        list<int> listaSolOrdenada;
             
        //Mientras queden datos en el vector de aportes
        while(!aportes.empty()){
            ordenacionPorAporte(listaSolucion, aportes, listaSolOrdenada, aportesOrdenado);
        }

        //BUSQUEDA DE MEJOR VECINO
        auto iter = aportesOrdenado.begin();
        bool mejora = false;

            //Si no se ha encontrado un vecindario mejor en el elemento que menos aporta
            //se pasa a buscar un mejor vecindario en el siguiente elemento que menos aporta
            //, así hasta que hayamos probado con todos los elementos o hayamos encontrado
            // una mejor solucion.
            while((iter != aportesOrdenado.end()) && !mejora){

                auto costeMinimo = *iter;
                int indiceMinimo = listaSolOrdenada.front();
                listaSolOrdenada.pop_front();

                //Desordenamos la lista de Abiertos_BL para no favorecer siempre a los
                //primeros elementos
                barajar(listaCandidatos);

                //Buscamos en todo el vecindario hasta encontrar una mejor solucion
                
                for(auto iterator = listaCandidatos.begin(); iterator!=listaCandidatos.end() && !mejora; iterator++){
                    
                    double costeMejor = 0;

                    //Calculo del aporte del candidato al que apunte iterator
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

                //Si no se ha encontrado una mejor solucion, incluimos el elemento de menor
                //aporte otra vez la lista Solucion_BL_ordenado y probamos con el siguiente
                //elemento
                if(!mejora){
                    listaSolOrdenada.push_back(indiceMinimo);
                    iter++;
                }
            }
            //Si no hemos encontrado una mejor solucion y hemos probado cambiar todos los
            //elementos, se finaliza el algoritmo
            if(!mejora){
                parada = true;
            }
            listaSolucion = listaSolOrdenada;
            cambio++;
        }
}

int main(int argc, char *argv[]){

	int n, m, k, l, LIMITE=10000;
	double distancia;
	if(argc!=3){
    		printf("Datos mal introducidos.\n");
    		exit(1); 
	}

	string file_name = argv[1];	//Fichero del que tomar datos


		//Fichero en el que escribir datos

	int Semilla = stoi(argv[2]);	//Semilla aleatoria
	Set_random(Semilla);

	ifstream File;

	File.open("../datos/"+file_name);	

	if(File.fail()){
		cout<<"Error al abrir el archivo, por favor, compruebe el nombre del fichero de entrada..."<<endl;
		exit(1);
	}else{

		File >> n >> m;	

        auto numIteraciones = n*(n-1)/2;
        auto maxIteraciones = 10000;
        auto numAleatorios = 10;

		vector<vector<double>> vecDistancias (n);

        for(auto i = 0; i < n; i++)
            vecDistancias[i].resize(n);

        for(auto i = 0; i<numIteraciones; i++){
            File >> k >> l >> distancia;
            vecDistancias[k][l]=distancia;
            vecDistancias[l][k]=distancia;
        }


		File.close();

        auto Semilla2 = Get_random();

        cout<<"USANDO SEMILLA: "<<Semilla2<<endl;
		cout<<"Tamanio del problema: "<<n<<" "<<endl<<"Tamanio del espacio de soluciones: "<<m<<endl;
    
        
        list<list<int>> matrizSolucion, matrizCandidatos;

        auto hastaRellenar = 0;
        
        for(auto i=0; i<numAleatorios; i++){
            list<int> listaSolucion;
            for(int j = 0; hastaRellenar < m; j++){            
                auto iRandom = Randint(-1,n-1);
                if(!estaDentro(iRandom,listaSolucion)){     //Necesario para no meter numeros repetidos que no sean borrados en candidatos.
                    listaSolucion.push_back(iRandom);
                    hastaRellenar++;
                }
            }
            matrizSolucion.push_back(listaSolucion);
            hastaRellenar = 0;
        }

        for(auto it=matrizSolucion.begin(); it!=matrizSolucion.end(); ++it){
            list<int> listaCandidatos;
            for(auto j=0;j<n;++j){
                if(!estaDentro(j, (*it))){
                    listaCandidatos.push_back(j);
                }
            }
            matrizCandidatos.push_back(listaCandidatos);
        }

        auto t0=clock();

        for(auto it=matrizSolucion.begin(), it2 = matrizCandidatos.begin(); it!=matrizSolucion.end(); ++it, ++it2){
            BusquedaLocal(vecDistancias, m, LIMITE, (*it), (*it2));
        }

        list<double> costes;

        for(auto it=matrizSolucion.begin(); it!=matrizSolucion.end(); ++it){
            costes.push_back(calculoCosteFinal((*it), vecDistancias));
        }

        auto t1=clock();
        
	    auto time = (double(t1-t0)/CLOCKS_PER_SEC);

	    cout << "Execution Time: " << time << endl;

	for(auto it=costes.begin(); it!=costes.end(); ++it){
		cout<<"Coste: "<<(*it)<<endl;
	}
	

    }
}
