#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;


bool estaDentro(int j, vector<int> vecSolucion){

	for(auto i=0;i<vecSolucion.size();i++){
		if(j==vecSolucion[i]) return true;
	}
	return false;
}


int main(){

	int n, m, contador=0, k, l;
	double distancia;
	

	ifstream File("C:/Users/ivanc/OneDrive/Desktop/Universidad/MH/Instancias y Tablas MDP 2019-20/MDG-b_1_n500_m50.txt",ios::in);

	if(!File){
		cout<<"Error al abrir el archivo, por favor, compruebe el nombre del fichero de entrada..."<<endl;
		exit(1);
	}else{

		File >> n >> m;	

		vector<vector<double>> vecDistancias (n, vector<double> (n));
		vector<int> vecSolucion;

		//Redimension del vector de seleccionados

		cout<<"Tamanio del problema: "<<n<<" "<<endl<<"Tamanio del espacio de soluciones: "<<m<<endl;

		
		for(auto i=0; i<n-1 ;i++){
			for(auto j=i+1;j<n; j++){

				File >> k >> l >> distancia;			
				vecDistancias[i][j] = distancia;						
				vecDistancias[j][i] = distancia;
			}			
		}

		for(auto i=0,j=0;i<n-1; i++,j++){
			vecDistancias[i][j] = 0;
	
		}

		cout<<"Distancias importadas correctamente... "<<vecSolucion.size()<<endl;
		
		File.close();

		

		auto valor=0.0, anterior=0.0;
		auto iBuena=0;

		auto t0=clock();

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


				cout<<contador<<endl;


				contador++;

			}

		auto t1=clock();

		auto time = (double(t1-t0)/CLOCKS_PER_SEC);

		cout << "Execution Time: " << time << endl;	

		auto costeTotal=0.0;
		
		for(auto i=0; i<m-1;i++){
			for(auto j=i+1;j<m;j++){
				costeTotal+=vecDistancias[vecSolucion[i]][vecSolucion[j]];					
			}	
		}
		

		cout<<"COSTE : "<<costeTotal<<endl;
		


	}

	

	return 0;

}