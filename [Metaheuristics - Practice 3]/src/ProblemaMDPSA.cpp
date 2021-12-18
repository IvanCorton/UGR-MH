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

int main(int argc, char *argv[]){

	int n, m, k, l, LIMITE=50;
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
        auto maxIteraciones = 100000;

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
    
        list<int> listaSolucion, mejorSolucion;

        auto hastaRellenar = 0;

        for(int j = 0; hastaRellenar < m; j++){            
            auto iRandom = Randint(-1,n-1);
            if(!estaDentro(iRandom,listaSolucion)){     //Necesario para no meter numeros repetidos que no sean borrados en candidatos.
                listaSolucion.push_back(iRandom);
                hastaRellenar++;
            }
        }

        

        auto costeActual = calculoCosteFinal(listaSolucion, vecDistancias);
       
       // Definicion de Temperatura Inicial y Final.
        
        auto mu = 0.3, fi = 0.3, alpha = 0.90;
        double numerador = mu * costeActual;
        double denominador = -log(fi);

        auto Tinicial = numerador / denominador;
        auto Tfinal = pow(10.0, -3.0);

        // Definicion de condiciones de salida del For Interno.

        int maxVecinos=10*n, numVecinos=0; // tamaño del caso del problema  
        int maxExitos=0.1*maxVecinos, numExitos=0;
        int maxEnfriamientos = maxIteraciones / maxVecinos;  // Numero de repeticiones Bucle Exterior.
        int numEnfriamientos = 0;
        bool enfriar = true;

        auto Tactual = Tinicial;

        mejorSolucion = listaSolucion;

        auto t0=clock();

        do{

            while(numVecinos < maxVecinos && numExitos < maxExitos && enfriar){

                list<int> vecino = intAleatorio(listaSolucion, n);
                
                costeActual = calculoCosteFinal(listaSolucion, vecDistancias);
                auto costeCandidato = calculoCosteFinal(vecino, vecDistancias);

                auto diferencia = costeCandidato - costeActual;

                (diferencia==0) ? (diferencia+=0.1) : (diferencia=diferencia);

                auto aleatorio = Randfloat(0,1);

                if((diferencia>0) || (aleatorio >= exp((-diferencia)/Tactual))){
                    listaSolucion = vecino;

                    auto costeAprueba = calculoCosteFinal(listaSolucion, vecDistancias);
                    auto costeOptimo = calculoCosteFinal(mejorSolucion, vecDistancias);

                    if(costeAprueba > costeOptimo){
                        mejorSolucion = listaSolucion;
                        numExitos++;
                    }
                }

                numVecinos++;

                if(numVecinos==maxVecinos-1 && numExitos==0){
                    enfriar = false;
                }

                cout<<"Exitos: "<<numExitos<<" Vecinos: "<<numVecinos<<endl;

            }

            Tactual *= alpha;
            
            numEnfriamientos++;

            numVecinos = 0, numExitos = 0;

        }while((Tactual>=Tfinal) && (numEnfriamientos < maxEnfriamientos));

        auto t1=clock();
        
	    auto time = (double(t1-t0)/CLOCKS_PER_SEC);

	    cout << "Execution Time: " << time << endl;

        double calculoFinal = calculoCosteFinal(mejorSolucion, vecDistancias);

        cout<<"Calculo total: " << calculoFinal << endl;

        t1 = 0;

    }

    return 0;
}
