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
#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9
#define MAX 2000000000000000

using namespace std;

unsigned long Seed = 0L;

/* GENERADOR DE NUMEROS ALEATORIOS */

/* Inicializa la semilla al valor 'x'.
   Solo debe llamarse a esta funcion una vez en todo el programa */
void Set_random (unsigned long x){
    Seed = (unsigned long) x;
}

/* Devuelve el valor actual de la semilla */
unsigned long Get_random (void){
    return Seed;
}

/* Genera un numero aleatorio real en el intervalo [0,1[
   (incluyendo el 0 pero sin incluir el 1) */
float Rand(void){
    return (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE );
}

/* Genera un numero aleatorio entero en {low,...,high} */
int Randint(int low, int high){
    return (int) (low + (high-(low)+1) * Rand());
}

/* Genera un numero aleatorio real en el intervalo [low,...,high[
   (incluyendo 'low' pero sin incluir 'high') */
float Randfloat(float low, float high){
    return (low + (high-(low))*Rand());
}

void barajar(list<int> &lista){
  vector <int> listovec(lista.begin(), lista.end());
  random_shuffle(listovec.begin(), listovec.end());
  copy(listovec.begin(), listovec.end(), lista.begin());
}

bool estaDentro(int j, list<int> vecSolucion){

	for(auto it=vecSolucion.begin();it!=vecSolucion.end();it++){
		if((*it)==j) return true;
	}
	return false;
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

double calculoCosteFinal(list<int> listaSolucion, vector<vector<double>> vecDistancias){

    auto costeFinal = 0.0;

  for(list<int>::iterator i = listaSolucion.begin(); i != listaSolucion.end(); i++){
    for(list<int>::iterator j = i; j != listaSolucion.end(); j++){
      costeFinal += vecDistancias[*i][*j];
    }
  }
  return costeFinal;
}



int main(){

	int n, m, k, l, LIMITE=100000;
	double distancia;
	
    auto Semilla = 1234567890;
    Set_random(Semilla);

	ifstream File("C:/Users/ivanc/OneDrive/Desktop/Universidad/MH/Instancias y Tablas MDP 2019-20/GKD-c_11_n500_m50.txt",ios::in);

	if(!File){
		cout<<"Error al abrir el archivo, por favor, compruebe el nombre del fichero de entrada..."<<endl;
		exit(1);
	}else{

		File >> n >> m;	

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

		list<int> listaSolucion, listaCandidatos;

        auto Semilla2 = Get_random();
        cout<<"USANDO SEMILLA: "<<Semilla2<<endl;

		cout<<"Tamanio del problema: "<<n<<" "<<endl<<"Tamanio del espacio de soluciones: "<<m<<endl;
		
        // RELLENAR VECTOR DE CANDIDATOS
        for(int i = 0; i < n; i++){
            listaCandidatos.push_back(i);
        }

		// CREAR UNA PRIMERA SOLUCION ALEATORIA (VARIACION EN FUNCION DE SEMILLA GENERADA)
		for(int i = 0; hastaRellenar < m; i++){            
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

        auto costeFinal = calculoCosteFinal(listaSolucion, vecDistancias);
            
        auto t1=clock();
        
	    auto time = (double(t1-t0)/CLOCKS_PER_SEC);

	    cout << "Execution Time: " << time << endl;

	    cout<<"COSTE FINAL: "<<costeFinal<<endl;
        
    }

	return 0;

}