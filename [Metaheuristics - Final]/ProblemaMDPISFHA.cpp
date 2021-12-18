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


bool estaDentro(int j, list<int> listaSolucion){
	list<int>::iterator it=listaSolucion.begin();
    
    for(auto it=listaSolucion.begin();it!=listaSolucion.end(); ++it){
        if(*it==j){
            return true;
        }
    }

    return false;

}


void barajar(list<int> &lista){
  vector <int> listovec(lista.begin(), lista.end());
  random_shuffle(listovec.begin(), listovec.end());
  copy(listovec.begin(), listovec.end(), lista.begin());
}


void reemplazar(list<int> desde, list<int> &hacia, int posicion){

    auto it1 = desde.begin(), it2 = hacia.begin();

    advance(it1,posicion);

    advance(it2,posicion);

    (*it2) = (*it1); 

}


void mutacion(list<list<int>> &poblacionIntermedia){

    int m = poblacionIntermedia.size();
    auto it = poblacionIntermedia.begin();
    int n = (*it).size();

    int numGenesTotal = n * m;

    int numEsperadoMutaciones = 0.01 * numGenesTotal;

    while(numEsperadoMutaciones>0){

        auto Cromosoma = Randint(-1,m-1);
        auto GenAmutar = Randint(-1,m-1);           //Gen elegido para ser cambiado.
        auto GenMutado = 0;

        auto iterPoblacion = poblacionIntermedia.begin();
        int aux = 0;

        advance(iterPoblacion,Cromosoma);


        do{
            GenMutado = Randint(-1,n-1);           //Gen por el que se cambia.
        }while(estaDentro(GenMutado, *iterPoblacion));

        auto jt = (*iterPoblacion).begin(); 

        advance(jt, GenAmutar);
        
        (*jt) = GenMutado;

        numEsperadoMutaciones--;

    }

}


list<list<int>> torneoBinario(list<list<int>> poblacionInicial, list<list<double>> aportes, int tam, int numPadres){

    int contador = 0;
    list<list<int>> padres;

    while(contador<numPadres){

        //Seleccion competidores
        
        auto iRandom = Randint(-1,tam-1);
        auto jRandom = Randint(-1,tam-1);

        while(iRandom==jRandom){
            jRandom = Randint(-1,tam-1);
        }
    
        list<list<int>>::iterator iteradorLista = poblacionInicial.begin();
        advance(iteradorLista,iRandom);

        list<int> candidato1 = (*iteradorLista);

        iteradorLista = poblacionInicial.begin();
        advance(iteradorLista,jRandom);

        list<int> candidato2 = (*iteradorLista);

        //Torneo

        list<list<double>>::iterator iteradorAportes1, iteradorAportes2;
        iteradorAportes1 = aportes.begin(); advance(iteradorAportes1, iRandom);
        iteradorAportes2 = aportes.begin(); advance(iteradorAportes2,jRandom);

        list<double> aportes1, aportes2;

        aportes1 = (*iteradorAportes1);
        aportes2 = (*iteradorAportes2);

        double aporteTotal1 = 0.0, aporteTotal2 = 0.0;

        auto it2=aportes2.begin();

        for(auto it=aportes1.begin(); it!=aportes1.end(); it++, it2++){
            aporteTotal1+=(*it);
            aporteTotal2+=(*it2);
        }

        if(aporteTotal1>aporteTotal2){
            padres.push_back(candidato1);
        }else{
            padres.push_back(candidato2);
        }

        contador++;

    }

    return padres;

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


bool compare(const pair<int, int>&i, const pair<int, int>&j){
    return i.first > j.first;
}

void aniadirMejores(list<double> &aportes, list<int> &candidatos, list<int> &candidatosSol, list<int> &hijo, int &numGenes, int m){

    vector<pair<double,int>> vector;
    double aporteX;
    int candidatoX, contador=0;

    list<int>::iterator itC = candidatos.begin();

    for(list<double>::iterator it=aportes.begin(); it!=aportes.end();it++, itC++){
        vector.push_back(make_pair((*it), (*itC)));
    }

    sort(vector.begin(),vector.end(), compare);

    hijo.push_back(vector.front().second);
    
    candidatosSol = hijo;

    numGenes++;

}


void quitarMejores(list<double> &aportes, list<int> &candidatos, int &numGenes, int m){

    vector<pair<double,int>> vector;
    double aporteX;
    int candidatoX;

    aportes.resize(numGenes);

    list<int>::iterator itC = candidatos.begin();

    for(list<double>::iterator it=aportes.begin(); it!=aportes.end();it++, itC++){
        vector.push_back(make_pair((*it), (*itC)));
    }

    sort(vector.begin(),vector.end());

    vector.resize(m);

    auto it2 = candidatos.begin();
    auto it = aportes.begin();

    for(auto i=0; i<m; i++, it++, it2++){
        (*it) = vector[i].first;
        (*it2) = vector[i].second;
    }

    aportes.resize(m); candidatos.resize(m);

    numGenes = aportes.size();

}




void reparacion(list<int> &hijo, vector<vector<double>> vecDistancias, list<double> &aporteHijo, int tam, int n){

    int numGenes = hijo.size(), contador=0;
    list<double> aportes;

    aportes.assign(numGenes, 0);

    if(tam - numGenes == 0){

        vector<double> listovec(aportes.size());
                                    
            auto i = 0, j = 0;

            for(list<int>::iterator it = hijo.begin(); it != hijo.end(); it++, i++){
                j = i;
                for(list<int>::iterator jt = it; jt != hijo.end(); jt++, j++){
                    listovec[i] += vecDistancias[*it][*jt];
                    listovec[j] += vecDistancias[*it][*jt];
                }
            }

            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());
            aportes.resize(tam);

        aporteHijo = aportes;

    }else if(tam-numGenes<0){        //Si sobran elementos eliminaremos los de mayor aporte.

            vector<double> listovec(aportes.size());
                                    
            auto i = 0, j = 0;

            for(list<int>::iterator it = hijo.begin(); it != hijo.end(); it++, i++){
                j = i;
                for(list<int>::iterator jt = it; jt != hijo.end(); jt++, j++){
                    listovec[i] += vecDistancias[*it][*jt];
                    listovec[j] += vecDistancias[*it][*jt];
                }
            }

            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());

            quitarMejores(aportes, hijo, numGenes, tam);

            aporteHijo = aportes;

            hijo.sort();

            //CONSEGUIR QUE SE VAYAN MARCANDO LOS NUMEROS DE CANDIDATOS
            /*contador = 0;

            auto candidatoI = candidatos.front();

            for(auto i=hijo.begin(); i!=hijo.end(); i++, contador++){
                if(contador==candidatoI){
                    (*i) = 1;
                    candidatos.pop_front();
                    candidatoI = candidatos.front();
                }else{
                    (*i) = 0;
                }
            }*/

    }else{
        
        while(tam-numGenes > 0){
                contador=0, numGenes=hijo.size();
                int numCeros = 0;
                list<int> candidatos, candidatosSol;

                candidatosSol = hijo;

                for(auto i=0; i<n; i++){
                    if(!estaDentro(i, hijo)){
                        candidatos.push_back(i);
                    }
                }

                aportes.assign(candidatos.size(), 0);

                /*vector<double> listovec(aportes.size());

               

            for(auto i=0; i<candidatos.size(); i++, iteradorCand++){
                for(auto iteradorSol = candidatosSol.begin(); iteradorSol!=candidatosSol.end(); iteradorSol++){
                    listovec[i]+=vecDistancias[*iteradorCand][*iteradorSol];
                }
            }
*/

            vector<double> listovec(aportes.size());
                                    
            auto i = 0, j = 0;
            auto iteradorCand = candidatos.begin();
            auto iteradorSol = candidatosSol.begin();

            for(list<int>::iterator it = candidatos.begin(); it != candidatos.end(); it++, i++){
                for(list<int>::iterator jt = candidatosSol.begin(); jt != candidatosSol.end(); jt++){
                    listovec[i] += vecDistancias[*it][*jt];
                }
            }


            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());

            aportes.resize(n-hijo.size());

            aniadirMejores(aportes, candidatos, candidatosSol ,hijo, numGenes, tam);

            aportes.assign(tam, 0);

            vector<double> listovec2(aportes.size());

            auto ia = 0, ij = 0;

            for(auto iteradorSol2 = candidatosSol.begin(); iteradorSol2!=candidatosSol.end(); iteradorSol2++, ia++){
                ij = ia;
                for(auto iter2 = iteradorSol2; iter2 != candidatosSol.end(); iter2++, ij++){
                    listovec2[ia]+=vecDistancias[*iteradorSol2][*iter2];
                    listovec2[ij]+=vecDistancias[*iteradorSol2][*iter2];
                }
            }

            aportes.insert(aportes.begin(), listovec2.begin(), listovec2.end());

            aportes.resize(numGenes);


        }

        aporteHijo = aportes;

    }



}


//Genera soluciones factibles pero menos convergentes.
void positionCrossover(list<int> padre1, list<int> padre2, list<int> &hijo1, list<int> &hijo2, int tam){

    int contador=0;
    list<int> hijoParcial1, hijoParcial2;
    list<int> restosPadre1, restosPadre2;

    hijoParcial1.assign(tam,-1);

    for(auto it1 = padre1.begin(), it2 = padre2.begin(), it3 = hijoParcial1.begin(); it1!=padre1.end(); it1++, it2++, it3++, contador++){
        if((*it1)==(*it2)){
            reemplazar(padre1, hijoParcial1, contador);
        }else{
            restosPadre1.push_back((*it1));
        }
    }

    hijoParcial2 = hijoParcial1;
    restosPadre2 = restosPadre1;

    barajar(restosPadre1);

    for(auto it1 = hijoParcial1.begin(); it1!=hijoParcial1.end();it1++){
        if((*it1)==-1){
            (*it1) = restosPadre1.front();
            restosPadre1.pop_front();
        }
    }

    barajar(restosPadre2);

    for(auto it1 = hijoParcial2.begin(); it1!=hijoParcial2.end();it1++){
        if((*it1)==-1){
            (*it1) = restosPadre2.front();
            restosPadre2.pop_front();
        }
    }
    
    hijo1 = hijoParcial1;
    hijo2 = hijoParcial2;

}


void singlePointCrossover(list<int> padre1, list<int> padre2, list<int> &hijo1, int tam){

    list<int> hijoParcial1;
    int contador=0;

    hijoParcial1.assign(tam,-1);

    auto puntoCorte = Randint(0,tam);   
    
    // ASUMIMOS PRIMERA MITAD DE PADRE1 Y SEGUNDA DE PADRE2

    auto it = hijoParcial1.begin();
    auto it2 = padre1.begin();
    auto it3 = padre2.begin();
    advance(it2, puntoCorte);
    advance(it3, puntoCorte);

    hijoParcial1.insert(it, padre1.begin(), it2);

    hijoParcial1.insert(it, it3, padre2.end());
    
    hijoParcial1.resize(tam);

    hijo1 = hijoParcial1;   

}

// Necesita reparacion
void singlePointCrossover(list<int> padre1, list<int> &hijo1, int tam){

    list<int> subLista1, subLista2;

    auto puntoCorteMitad = tam/2; 
    
    auto puntoCorte1 = Randint(0, puntoCorteMitad);
    auto puntoCorte2 = Randint(puntoCorteMitad, tam);  
    
    auto it = subLista1.begin(), jt = subLista2.begin();
    auto itMitad = padre1.begin();
    advance(itMitad, puntoCorteMitad);

    subLista1.insert(it, padre1.begin(), itMitad);
    subLista2.insert(jt, itMitad, padre1.end());

    // BUSCAMOS CRUCE PRIMER SUBCROMOSOMA CON EL SEGUNDO.
    singlePointCrossover(subLista1, subLista2, hijo1, puntoCorteMitad);

}



//Genera soluciones no factibles. (requiere reparador)
void uniformCrossover(list<int> padre1, list<int> padre2, list<int> &hijo1, int tam){
   
    list<int> hijoParcial1;
    int contador=0;

    hijoParcial1.assign(tam,-1);

    for(auto it1 = padre1.begin(), it2 = padre2.begin(), it3 = hijoParcial1.begin(); it1!=padre1.end(); it1++, it2++, it3++, contador++){
        if((*it1)==(*it2)){
            reemplazar(padre1, hijoParcial1, contador);
        }
    }

    contador = 0;

    for(auto it1 = hijoParcial1.begin(); it1!=hijoParcial1.end();it1++, contador++){
        if((*it1)==-1){
            auto iRandom = Randint(0,100);
            if(iRandom<100/2)
                reemplazar(padre1, hijoParcial1, contador);
            else
                reemplazar(padre2, hijoParcial1, contador);
        }
    }

    hijo1 = hijoParcial1;

}



void recuperacion(list<list<int>> &pobIntermedia, list<list<double>> &aportIntermedia, list<list<int>> &copiaPapis, vector<vector<double>> vecDistancias, int tamPoblacion, int n){

    list<double> aporteHijo;
    //GENERACIONAL 1: SELECCION (Misma poblacion)
    list<int> padre1 = copiaPapis.front();
    copiaPapis.pop_front();
    list<int> padre2 = copiaPapis.front();
    copiaPapis.pop_front();
    list<int> hijo1, hijo2;

    //GENERACIONAL 2: CRUCE UNIFORME (Mayor corvengencia)
    singlePointCrossover(padre1, padre2, hijo1, tamPoblacion);
    reparacion(hijo1, vecDistancias, aporteHijo, tamPoblacion, n);

    pobIntermedia.push_back(hijo1);
    aportIntermedia.push_back(aporteHijo);

    singlePointCrossover(padre2, padre1, hijo2, tamPoblacion);
    reparacion(hijo2, vecDistancias, aporteHijo, tamPoblacion, n);

    pobIntermedia.push_back(hijo2);
    aportIntermedia.push_back(aporteHijo);

}


void comprobarReemplazamiento(list<int> candidatoActualReal, double maxAnterior, list<list<double>> &listaAportes, list<list<int>> &poblacionIntermedia, vector<vector<double>> vecDistancias, int tam){

    list<list<int>> poblacionEntera;
    list<int> poblacionIesima;
    list<double> costesFinales;
    auto contando = 0;
    auto costeFinali = 0.0;
    bool recuperarPrimero = true;

    for(auto it=poblacionIntermedia.begin(); it!=poblacionIntermedia.end(); it++){
        poblacionIesima = (*it);

        contando=0;
        costeFinali = calculoCosteFinal(poblacionIesima, vecDistancias);
        costesFinales.push_back(costeFinali);
        poblacionEntera.push_back(poblacionIesima);
        poblacionIesima.clear();

        if(maxAnterior<costeFinali) recuperarPrimero = false;
    }


    if(recuperarPrimero){

        contando = 0;

        poblacionIesima = candidatoActualReal;

        costeFinali = calculoCosteFinal(poblacionIesima, vecDistancias);

        poblacionEntera.push_back(poblacionIesima);
        costesFinales.push_back(costeFinali);

        vector<pair<double, int>> vector2;

        contando = 0;

        for(auto it=costesFinales.begin(); it!=costesFinales.end(); it++,contando++){
            vector2.push_back(make_pair(*it, contando));
        }

        sort(vector2.begin(), vector2.end(), compare);

        vector2.resize(tam);

        pair<double,int> indiceAquitar = vector2.back();

        auto it = poblacionIntermedia.begin();

        advance(it, indiceAquitar.second);

        (*it) = candidatoActualReal;

        //---------------------------------------------

    }


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



int main(){

	int n, m, k, l, LIMITE=15;
	double distancia;
	
    auto Semilla = 123457890;
    Set_random(Semilla);


    // Introducir ruta absoluta del fichero de datos del problema
	//ifstream File("C:/Users/ivanc/OneDrive/Desktop/Universidad/MH/Instancias y Tablas MDP 2019-20/MDG-b_10_n500_m50.txt",ios::in);

    
	ifstream File("C:/Users/ivanc/OneDrive/Desktop/Universidad/MH/Instancias y Tablas MDP 2019-20/MDG-b_10_n500_m50.txt",ios::in);

	if(!File){
		cout<<"Error al abrir el archivo, por favor, compruebe el nombre del fichero de entrada..."<<endl;
		exit(1);
	}else{

		File >> n >> m;

        auto numIteraciones = n*(n-1)/2;

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
        
        list<list<int>> listaSolucion, listaCandidatos;
        list<int> candidatosI;
        list<list<double>> listaAportes;
        list<double> aportes;

		// CREAR UNA PRIMERA SOLUCION DE SOLUCIONES ALEATORIA (VARIACION EN FUNCION DE SEMILLA GENERADA)
        for(auto k=0;k<m;k++){
            
            aportes.assign(m,0);     

            auto hastaRellenar = 0;

            vector<double> listovec(aportes.size());

            list<int> candidatos;
            
            for(int j = 0; hastaRellenar < m; j++){            

                auto iRandom = Randint(-1,n-1);
                
                if(!estaDentro(iRandom,candidatos)){     //Necesario para no meter numeros repetidos que no sean borrados en candidatos.
                    //Eliminar el 0 en la posicion iRandom e insertar un 1.
                    //cout<<"Aleatorio generado: "<<iRandom<<endl;
                    candidatos.push_back(iRandom);
                    hastaRellenar++;
                }
            }
            
            auto i = 0, j = 0;

            for(list<int>::iterator it = candidatos.begin(); it != candidatos.end(); it++, i++){
                j = i;
                for(list<int>::iterator jt = it; jt != candidatos.end(); jt++, j++){
                    listovec[i] += vecDistancias[*it][*jt];
                    listovec[j] += vecDistancias[*it][*jt];
                }
            }

            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());
            aportes.resize(m);
            
            listaSolucion.push_back(candidatos);
            listaAportes.push_back(aportes);

        }

        

        // GENERACION POBLACION MEDIANTE PROCESO EVOLUTIVO.

        auto cambio = 0;

        auto t0=clock();

        auto crucesEsperados = 0.6 * m/2;        
    
        auto numPadres = m/2;

        while((cambio < LIMITE)){

            if(cambio==21) break;

            crucesEsperados = 0.6 * numPadres;
        
            list<list<int>> poblacionPadres, poblacionPadres2, copiaPadres;
            list<double> aportePadre;

            poblacionPadres = listaSolucion;

            copiaPadres = poblacionPadres;

            aportePadre.assign(copiaPadres.size(),0);

            auto itPadres = poblacionPadres.begin();
                        
            list<list<double>> aportesIntermedios;
            list<list<int>> poblacionIntermedia;

            //PoblacionPadres guarda los cromosomas de los padres.
            //listaSolucion guarda todo el REBAÑO

            while(crucesEsperados>0){
                
                list<double> aporteHijo;

                //SFHA 1: SELECCION (Misma poblacion)
                list<int> padre1 = poblacionPadres.front();
                poblacionPadres.pop_front();
                list<int> hijo1;

                //SFHA 2: CRUCE UN SOLO PUNTO (Nivel subcromosomas)
                singlePointCrossover(padre1, hijo1, m);
                reparacion(hijo1, vecDistancias, aporteHijo, m, n);

                poblacionIntermedia.push_back(hijo1);
                aportesIntermedios.push_back(aporteHijo);

                crucesEsperados--;
            }

            //SFHA 3: RECUPERACION HASTA M DESDE PADRE 1 Y PADRE 2
            while(poblacionIntermedia.size()<m){
               recuperacion(poblacionIntermedia, aportesIntermedios,copiaPadres,vecDistancias, m, n);
            }

            if(poblacionIntermedia.size()>m){
                poblacionIntermedia.resize(m);
            }

                        //SFHA 9: MUTACION SIMPLE (Nivel cromosomas)
            mutacion(poblacionIntermedia);

            // Usaremos una mutacion de UN SOLO PUNTO ya que la
            // mutacion inversa no tendria sentido en nuestro problema
            // por NO influir el ORDEN en la funcion fitness.

            // Mutacion inversa -> Problemas multiobjetivos

            crucesEsperados = 0.6 * m/2;

            //SFHA 6: SELECCION (Misma poblacion)       19538 con 2 torneos       19562 con 1º Torneo solo        19565.494380000004  sin torneos
            poblacionPadres2 = poblacionIntermedia;
            copiaPadres = poblacionPadres2;

            poblacionIntermedia.clear();
            aportesIntermedios.clear();

            while(crucesEsperados>0){
                
                list<double> aporteHijo;

               list<int> padre1 = poblacionPadres2.front();
                poblacionPadres2.pop_front();
                list<int> padre2 = poblacionPadres2.front();
                poblacionPadres2.pop_front();
                list<int> hijo1, hijo2;

                //SFHA 7: CRUCE DE UN SOLO PUNTO (Nivel cromosomas)
                singlePointCrossover(padre1, padre2, hijo1, m);    
                reparacion(hijo1, vecDistancias, aporteHijo, m, n);

                poblacionIntermedia.push_back(hijo1);
                aportesIntermedios.push_back(aporteHijo);

                singlePointCrossover(padre2, padre1, hijo2, m);
                reparacion(hijo2, vecDistancias, aporteHijo, m, n);

                poblacionIntermedia.push_back(hijo2);
                aportesIntermedios.push_back(aporteHijo);

                crucesEsperados--;
            }

            //SFHA 8: RECUPERACION HASTA M DESDE POBLACION INTERMEDIA
            while(poblacionIntermedia.size()<m){
               recuperacion(poblacionIntermedia, aportesIntermedios,copiaPadres,vecDistancias, m, n);
            }

            if(poblacionIntermedia.size()>m){
                poblacionIntermedia.resize(m);
            }



            //SFHA 10: REEMPLAZAMIENTO ELITISTA.

            //ORDENANDO APORTES INICIALES POR INDICES
            auto aporteA = 0.0;

            list<double> aportesIniciales;
            list<int> indicesSol;


            for(auto it = poblacionIntermedia.begin(); it!=poblacionIntermedia.end(); it++){
                aporteA = calculoCosteFinal(*it, vecDistancias);
                aportesIniciales.push_back(aporteA);
                aporteA = 0;
            }

            for(int i=0; i<m;i++){
                indicesSol.push_back(i);
            }

            vector<pair<double, int>> vector2;

            auto itC = indicesSol.begin();

            for(list<double>::iterator it=aportesIniciales.begin(); it!=aportesIniciales.end();it++, itC++){
                vector2.push_back(make_pair((*it), (*itC)));
            }

            sort(vector2.begin(),vector2.end(), compare);       //APORTES DE SOLINICIAL POR INDICE

            auto mejorCromosomaActual = vector2.front().second;

            auto itA = poblacionIntermedia.begin();

            advance(itA, mejorCromosomaActual);
            
            double maxAnterior = calculoCosteFinal((*itA), vecDistancias);

            comprobarReemplazamiento((*itA), maxAnterior, listaAportes, poblacionIntermedia, vecDistancias, m);

            listaSolucion = poblacionIntermedia;
            
            listaCandidatos = listaSolucion;

          //listaCandidatos
            
            bool heHechoBL = false;
            auto contadorBL=0;
            auto it = listaSolucion.begin();
            list<list<double>> aportesMejores;
            
                    listaCandidatos.clear();
                    for(auto it=listaSolucion.begin(); it!=listaSolucion.end(); ++it){
                        for(auto i=0; i<n; i++){
                            if(!estaDentro(i,(*it))){
                                candidatosI.push_back(i);
                            }
                        }
                        listaCandidatos.push_back(candidatosI);
                        candidatosI.clear();
                    }
            
                auto itCandidatos = listaCandidatos.begin();

            while(contadorBL<m && cambio%3==0 && cambio!=0){
                
                BusquedaLocal(vecDistancias, m, 400, (*it) , (*itCandidatos));

                auto contado = 0;

                list<int> mejorSolucionEntera = (*it);

                mejorSolucionEntera.sort();

                //ACTUALIZAR LISTA SOLUCION EN BINARIO Y APORTES.
                auto iterBin = mejorSolucionEntera.begin();

                list<double> aportesMejI;

                vector<double> listovec(m);

                auto i = 0, j = 0;

                for(list<int>::iterator it = mejorSolucionEntera.begin(); it != mejorSolucionEntera.end(); it++, i++){
                    j = i;
                    for(list<int>::iterator jt = it; jt != mejorSolucionEntera.end(); jt++, j++){
                        listovec[i] += vecDistancias[*it][*jt];
                        listovec[j] += vecDistancias[*it][*jt];
                    }
                }

                aportesMejI.insert(aportesMejI.begin(), listovec.begin(), listovec.end());
                aportesMejI.resize(m);
                
                aportesMejores.push_back(aportesMejI);

                it++; itCandidatos++;
                contadorBL++; heHechoBL=true;


            }

            if(heHechoBL) listaAportes = aportesMejores;

            cambio++; cout<<"cambio numero "<<cambio<<endl;

        }


        listaCandidatos.clear();

        auto contador=0, cont=0;
        bool valido = false;

        for(auto it=listaSolucion.begin(); it!=listaSolucion.end(); ++it, cont++){

            for(list<int>::iterator jt=(*it).begin(); jt!=(*it).end(); ++jt){
                valido=false;
                contador=0;
                for(list<int>::iterator jt2 = (*it).begin(); jt2!=(*it).end(); ++jt2){
                    if((*jt)==(*jt2))
                        contador++;
                }
                if(contador==1) valido=true;   
                if(valido == false) break;
            }
            if(valido==true){
                listaCandidatos.push_back(*it);
            }
            valido=false;
        }

        listaSolucion = listaCandidatos;

        auto t1=clock();
        
	    auto time = (double(t1-t0)/CLOCKS_PER_SEC);

	    cout << "Execution Time: " << time << endl;

        list<double> listaAportesReal;

        for(auto it=listaSolucion.begin(); it!=listaSolucion.end(); ++it){
            auto aporteIesimo = 0.0;
            aporteIesimo = calculoCosteFinal(*it,vecDistancias);
            listaAportesReal.push_back(aporteIesimo);
        }
  
    auto anterior = 0.0;

        for(auto it=listaAportesReal.begin(); it!=listaAportesReal.end(); ++it){
            if(*it>anterior)
                anterior = (*it);
        }

	    cout<<"COSTE FINAL: "<<anterior<<endl;
        
    }

	return 0;

}