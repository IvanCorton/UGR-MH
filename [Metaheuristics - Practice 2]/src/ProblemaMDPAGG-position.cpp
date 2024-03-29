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
#include "math.h"
#include "random.h"
using namespace std;


bool estaDentro(int j, list<int> vecSolucion){
	list<int>::iterator it=vecSolucion.begin();
    advance(it,j);
    if(*it!=1)
        return false;
    else
        return true;

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
        auto GenAmutar = Randint(-1,n-1);           //Gen elegido para ser cambiado.
        auto GenMutado = Randint(-1,n-1);           //Gen por el que se cambia.

        auto iterPoblacion = poblacionIntermedia.begin();
        int aux = 0;

        advance(iterPoblacion,Cromosoma);

        auto jt = (*iterPoblacion).begin(); 

        advance(jt, GenAmutar);

        auto ji = (*iterPoblacion).begin();

        advance(ji, GenMutado);

        if((*jt)!=(*ji)){
            aux = (*jt);
            (*jt) = (*ji);
            (*ji) = aux;
        }else{

            while((*jt)==(*ji)){
                GenMutado = Randint(-1,n-1);
                ji = (*iterPoblacion).begin();
                advance(ji,GenMutado);
            }

            aux = (*jt);
            (*jt) = !aux;
            (*ji) = aux;

        }

        numEsperadoMutaciones--;

    }

}


list<list<int>> torneoBinario(list<list<int>> &poblacionInicial, list<list<double>> aportes, int tam){

    int contador = 0;
    list<list<int>> padres;

    while(contador<tam){

        //Seleccion competidores

        auto iRandom = Randint(-1,tam-1);
        list<list<int>>::iterator iteradorLista = poblacionInicial.begin();
        advance(iteradorLista,iRandom);

        list<int> candidato1 = (*iteradorLista);

        auto jRandom = Randint(-1,tam-1);
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

    for(auto it=hijo.begin();it!=hijo.end();it++,contador++){
        if(contador==vector.front().second)
            (*it)=1;
    }

    candidatosSol.push_back(vector.front().second);

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




void reparacion(list<int> &hijo, vector<vector<double>> vecDistancias, list<double> &aporteHijo, int tam){

    int numGenes = 0, contador=0;
    list<double> aportes;

    for(auto it = hijo.begin(); it != hijo.end(); it++, contador++){
        if((*it)==1){
            numGenes++;
        } 
    }

    aportes.assign(numGenes, 0);

    if(tam - numGenes == 0){

        list<int> candidatos;

            contador=0;

            for(auto it = hijo.begin(); it != hijo.end(); it++, contador++){
                if((*it)==1){
                    candidatos.push_back(contador);
                } 
            }
        

        vector<double> listovec(aportes.size());
                                    
            auto i = 0, j = 0;

            for(list<int>::iterator it = candidatos.begin(); it != candidatos.end(); it++, i++){
                j = i;
                for(list<int>::iterator jt = it; jt != candidatos.end(); jt++, j++){
                    listovec[i] += vecDistancias[*it][*jt];
                    listovec[j] += vecDistancias[*it][*jt];
                }
            }

            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());
            aportes.resize(tam);

        aporteHijo = aportes;

    }else if(tam-numGenes<0){        //Si sobran elementos eliminaremos los de mayor aporte.
                               
            list<int> candidatos;

            contador=0;

            for(auto it = hijo.begin(); it != hijo.end(); it++, contador++){
                if((*it)==1){
                    candidatos.push_back(contador);
                } 
            }


            vector<double> listovec(aportes.size());
                                    
            auto i = 0, j = 0;

            for(list<int>::iterator it = candidatos.begin(); it != candidatos.end(); it++, i++){
                j = i;
                for(list<int>::iterator jt = it; jt != candidatos.end(); jt++, j++){
                    listovec[i] += vecDistancias[*it][*jt];
                    listovec[j] += vecDistancias[*it][*jt];
                }
            }

            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());

            quitarMejores(aportes, candidatos, numGenes, tam);

            aporteHijo = aportes;

            candidatos.sort();

            //CONSEGUIR QUE SE VAYAN MARCANDO LOS NUMEROS DE CANDIDATOS
            contador = 0;

            auto candidatoI = candidatos.front();

            for(auto i=hijo.begin(); i!=hijo.end(); i++, contador++){
                if(contador==candidatoI){
                    (*i) = 1;
                    candidatos.pop_front();
                    candidatoI = candidatos.front();
                }else{
                    (*i) = 0;
                }
            }

    }else{
        
        while(tam-numGenes > 0){
                contador=0, numGenes=0;
                int numCeros = 0;
                list<int> candidatos, candidatosSol;

                for(auto it = hijo.begin(); it != hijo.end(); it++, contador++){
                    if((*it)==0){
                        candidatos.push_back(contador); 
                        numCeros++;
                    }else{
                        candidatosSol.push_back(contador);
                        numGenes++;
                    }
                }

                aportes.assign(candidatos.size(), 0);

                vector<double> listovec(aportes.size());

                auto iteradorCand = candidatos.begin();
                auto iteradorSol = candidatosSol.begin();

            for(auto i=0; i<candidatos.size(); i++, iteradorCand++){
                for(auto iteradorSol = candidatosSol.begin(); iteradorSol!=candidatosSol.end(); iteradorSol++){
                    listovec[i]+=vecDistancias[*iteradorCand][*iteradorSol];
                }
            }

            aportes.insert(aportes.begin(), listovec.begin(), listovec.end());

            aportes.resize(numCeros);

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



void recuperacion(list<list<int>> &pobIntermedia, list<list<double>> &aportIntermedia, list<list<int>> &copiaPapis, vector<vector<double>> vecDistancias, int tam){

    list<double> aporteHijo;
    //GENERACIONAL 1: SELECCION (Misma poblacion)
    list<int> padre1 = copiaPapis.front();
    copiaPapis.pop_front();
    list<int> padre2 = copiaPapis.front();
    copiaPapis.pop_front();
    list<int> hijo1, hijo2;

    //GENERACIONAL 2: CRUCE UNIFORME (Mayor corvengencia)
    uniformCrossover(padre1, padre2, hijo1, vecDistancias.size());
    reparacion(hijo1, vecDistancias, aporteHijo, tam);

    pobIntermedia.push_back(hijo1);
    aportIntermedia.push_back(aporteHijo);

    uniformCrossover(padre1, padre2, hijo2, vecDistancias.size());
    reparacion(hijo2, vecDistancias, aporteHijo, tam);

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
        for(auto jt = (*it).begin(); jt!=(*it).end(); jt++, contando++){
            if(*jt==1)
            poblacionIesima.push_back(contando);
        
        }
        contando=0;
        costesFinales.push_back(calculoCosteFinal(poblacionIesima, vecDistancias));
        poblacionEntera.push_back(poblacionIesima);
        costeFinali = calculoCosteFinal(poblacionIesima, vecDistancias);
        poblacionIesima.assign(0,0);

        if(maxAnterior<costeFinali) recuperarPrimero = false;
    }


    if(recuperarPrimero){

        contando = 0;

            for(auto it=candidatoActualReal.begin(); it!=candidatoActualReal.end(); it++,contando++){
                if(*it==1){
                    poblacionIesima.push_back(contando);
                }
            }

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

            list<list<double>> listaAportesProb;

            auto quitarPeor = poblacionEntera.begin();

            advance(quitarPeor, indiceAquitar.second);

            (*quitarPeor) = poblacionIesima;

            poblacionEntera.resize(tam);

            while(!poblacionEntera.empty()){

                list<double> aportes;

                aportes.assign(tam,0);     

                vector<double> listovec(aportes.size());

                list<int> candidatos = poblacionEntera.front();
                poblacionEntera.pop_front();


                auto i = 0, j = 0;

                for(list<int>::iterator it = candidatos.begin(); it != candidatos.end(); it++, i++){
                    j = i;
                    for(list<int>::iterator jt = it; jt != candidatos.end(); jt++, j++){
                        listovec[i] += vecDistancias[*it][*jt];
                        listovec[j] += vecDistancias[*it][*jt];
                    }
                }

                aportes.insert(aportes.begin(), listovec.begin(), listovec.end());
                aportes.resize(tam);
                
                listaAportesProb.push_back(aportes);

            }

            listaAportes = listaAportesProb;

    }


}



int main(int argc,char *argv[]){

	int n, m, k, l, LIMITE=30;
	double distancia;
	int h = 0;

	int numArchivosAcargar = 9;

	if(argc!=4){
    		printf("Datos mal introducidos.\n");
    		exit(1); 
	}

	string file_name = argv[1];	//Fichero del que tomar datos

	string out_name = argv[2];

	while(h<numArchivosAcargar){
		

	
		cout<<file_name<<endl;
		cout<<out_name<<endl;

		//Fichero en el que escribir datos

	int Semilla = stoi(argv[3]);	//Semilla aleatoria
	Set_random(Semilla);

	ifstream File;

	if(file_name[0] == 'M'){
		file_name[6]++;
	}else{
		file_name[7]++;
	}

	File.open("/home/cphys-lucia.zapataechevarne/Ivan/P2/datos/"+file_name);	

	if(File.fail()){
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
        list<int> Posibilidades, Solucioni;
        list<list<double>> listaAportes;
        list<double> aportes;

        //Posibilidades incluye un vector con todos los candidatos posibles a 0.
        Posibilidades.assign(n, 0);

		// CREAR UNA PRIMERA SOLUCION DE SOLUCIONES ALEATORIA (VARIACION EN FUNCION DE SEMILLA GENERADA)
        for(auto k=0;k<m;k++){

            Solucioni = Posibilidades;
            
            aportes.assign(m,0);     

            auto hastaRellenar = 0;

            vector<double> listovec(aportes.size());

            list<int> candidatos;
            
            for(int j = 0; hastaRellenar < m; j++){            

                auto iRandom = Randint(-1,n-1);
                
                if(!estaDentro(iRandom,Solucioni)){     //Necesario para no meter numeros repetidos que no sean borrados en candidatos.
                    //Eliminar el 0 en la posicion iRandom e insertar un 1.
                    //cout<<"Aleatorio generado: "<<iRandom<<endl;
                    candidatos.push_back(iRandom);
                    list<int>::iterator iterator = Solucioni.begin();
                    advance(iterator, iRandom);
                    iterator = Solucioni.erase(iterator);
                    Solucioni.insert(iterator, 1);
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
            
            listaSolucion.push_back(Solucioni);
            listaAportes.push_back(aportes);

        }

        

        // GENERACION POBLACION MEDIANTE PROCESO EVOLUTIVO.


        auto cambio = 0;
        auto parada = false;

        list<list<int>> poblacionActual = listaSolucion;

        list<list<int>> poblacionPadres, candidatosPadres, copiaPadres;
        list<list<double>> aportesPadres;
        list<double> aportePadre;

        auto t0=clock();
            
        auto crucesEsperados = 0.6 * m/2;   //Prob. cruce * tam/2.

        while((cambio < LIMITE)){
        
            poblacionPadres = torneoBinario(poblacionActual, listaAportes, m);

            copiaPadres = poblacionPadres;

            aportePadre.assign(m,0);

            auto itPadres = poblacionPadres.begin();

            while(itPadres!=poblacionPadres.end()){

                auto cuentaPadres = 0;
                list<int> candidatoPa;

                for(auto it = (*itPadres).begin();it!=(*itPadres).end(); it++, cuentaPadres++){
                    if(*it==1){
                        candidatoPa.push_back(cuentaPadres);
                    }
                }

                //CandidatoPa ya rellenado de candidatos, ahora rellenar aportePadre.
                auto i = 0, j = 0;

                vector<double> listovec(aportePadre.size());

                for(list<int>::iterator it = candidatoPa.begin(); it != candidatoPa.end(); it++, i++){
                    j = i;
                    for(list<int>::iterator jt = it; jt != candidatoPa.end(); jt++, j++){
                        listovec[i] += vecDistancias[*it][*jt];
                        listovec[j] += vecDistancias[*it][*jt];
                    }
                }

                aportePadre.insert(aportePadre.begin(), listovec.begin(), listovec.end());
                aportePadre.resize(m);

                candidatosPadres.push_back(candidatoPa);
                aportesPadres.push_back(aportePadre);
                itPadres++;

            }
                        
            list<list<double>> aportesIntermedios;
            list<list<int>> poblacionIntermedia;

            //CandidatosPadres guarda los indices que forman cada cromosoma.
            //AportesPadres guarda el aporte de cada gen de cada cromosoma.
            //PoblacionPadre guarda los cromosomas de los padres.

            while(crucesEsperados>0){
                
                list<double> aporteHijo;
                //GENERACIONAL 1: SELECCION (Misma poblacion)
                list<int> padre1 = poblacionPadres.front();
                poblacionPadres.pop_front();
                list<int> padre2 = poblacionPadres.front();
                poblacionPadres.pop_front();
                list<int> hijo1, hijo2;

                positionCrossover(padre1, padre2, hijo1, hijo2, n);
                reparacion(hijo1, vecDistancias, aporteHijo, m);
                
                poblacionIntermedia.push_back(hijo1);
                aportesIntermedios.push_back(aporteHijo);

                reparacion(hijo2, vecDistancias, aporteHijo, m);

                poblacionIntermedia.push_back(hijo2);
                aportesIntermedios.push_back(aporteHijo);

                //GENERACIONAL 3: RECUPERACION HASTA M DESDE PADRE 1 Y PADRE 2

                crucesEsperados--;
            }

            //GENERACIONAL 3: RECUPERACION HASTA M DESDE PADRE 1 Y PADRE 2
            while(poblacionIntermedia.size()<m){
               recuperacion(poblacionIntermedia, aportesIntermedios,copiaPadres,vecDistancias,m);
            }

            //GENERACIONAL 4: MUTACION (Mutar hijos)
            mutacion(poblacionIntermedia);

            //GENERACIONAL 5: REEMPLAZAMIENTO ELITISTA.

            auto aporteA = 0.0;

            list<double> aportesIniciales;

            for(auto it = listaAportes.begin(); it!=listaAportes.end(); it++){
                for(auto it2 = (*it).begin(); it2!=(*it).end(); it2++){
                    aporteA+=(*it2);
                }
                aportesIniciales.push_back(aporteA);
                aporteA = 0;
            }

            auto contadorA = 0;

            list<int> indicesSol;

            for(int i=0; i<m;i++){
                indicesSol.push_back(i);
            }

            vector<pair<double, int>> vector;

            auto itC = indicesSol.begin();

            for(list<double>::iterator it=aportesIniciales.begin(); it!=aportesIniciales.end();it++, itC++){
                vector.push_back(make_pair((*it), (*itC)));
            }

            sort(vector.begin(),vector.end(), compare);

            auto mejorCromosomaActual = vector.front().second;

            auto itA = listaSolucion.begin();

            advance(itA, mejorCromosomaActual);

            list<int> candidatoActual, candidatoActualReal;

            auto contando = 0;

            for(auto it = (*itA).begin(); it!=(*itA).end(); it++,contando++){
                if(*it==1){
                    candidatoActual.push_back(contando);
                }
                candidatoActualReal.push_back(*it);
            }

            double maxAnterior = calculoCosteFinal(candidatoActual, vecDistancias);

            comprobarReemplazamiento(candidatoActualReal, maxAnterior, listaAportes, poblacionIntermedia, vecDistancias, m);

            listaSolucion = poblacionIntermedia;
            
            cambio++; cout<<"cambio numero "<<cambio<<endl;

        }
            
        auto t1=clock();
        
	    auto time = (double(t1-t0)/CLOCKS_PER_SEC);

	    cout << "Execution Time: " << time << endl;

        list<int> listaSolEnteraI;
        list<list<int>> listaSolEntera;

        auto aporteIesimo = 0.0;
        list<double> listaAportesReal;

        auto contador = 0;

        for(auto it = listaSolucion.begin(); it!=listaSolucion.end(); it++){
            for(auto it2 = (*it).begin(); it2!=(*it).end(); it2++,contador++){
                if(*it2==1){
                    listaSolEnteraI.push_back(contador);
                }
            }
            contador=0;
            listaSolEntera.push_back(listaSolEnteraI);
            listaSolEnteraI.assign(0,0);
        }

        for(auto it= listaSolEntera.begin(); it!= listaSolEntera.end(); it++){
            aporteIesimo = calculoCosteFinal(*it, vecDistancias);
            listaAportesReal.push_back(aporteIesimo);
        }

fstream fichero("/home/cphys-lucia.zapataechevarne/Ivan/P2/datos/"+out_name, ios::app);

	//string unString;

 fichero <<"-------------------------------------------------------------------"<<endl;

        for(auto it=listaAportesReal.begin(); it!=listaAportesReal.end(); ++it){
            fichero << (*it); 
            fichero << endl;
        }
        
        fichero <<"Time: "<<time<<endl;
        fichero << "-------------------------------------------------------------------"<<endl;
        fichero.close();

	cout<<"COSTE FINAL: "<<endl;
	
	if(out_name[0] == 'M'){
		out_name[4]++;
	}else{
		out_name[5]++;
	}
		h++;
        
    }		//Fin while
        
    }

	return 0;

}
