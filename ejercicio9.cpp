#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std; 

//uso de nombres mnemotecnicos para facilitar casos. 
int ultimoMomento(int cantArchivos, int tamanio, int cantLineas, int* tamanios, int* puntos, int* arrLineas){ 
   int*** tab = new int**[cantArchivos + 1]; 
   for(int archivoActual= 0;archivoActual <=cantArchivos; archivoActual ++ ) {
      tab[archivoActual] = new int*[tamanio+1](); 
        for(int tamanioActual = 0; tamanioActual <= tamanio; tamanioActual++) {
           tab[archivoActual][tamanioActual] = new int[cantLineas+1]();
        }
      };
        for(int archivoActual = 1 ; archivoActual <= cantArchivos; archivoActual++) {
            int puntosArchivoActual = puntos[archivoActual - 1];   
            int tamanioArchivoActual = tamanios[archivoActual - 1]; 
            int lineasArchivoActual =  arrLineas[archivoActual - 1]; 

             for(int capacidadTamanioActual = 0; capacidadTamanioActual <= tamanio; capacidadTamanioActual++ ) {
                 for(int cantLineasActual  = 0; cantLineasActual <=cantLineas ; cantLineasActual++) {
                    if(tamanioArchivoActual > capacidadTamanioActual || lineasArchivoActual > cantLineasActual) {
                        tab[archivoActual] [capacidadTamanioActual] [cantLineasActual] = 
                        tab[archivoActual - 1][capacidadTamanioActual][cantLineasActual]; //para que se vea. 
                    }
                    else { 
                        int valorUsarArchivo = puntosArchivoActual + 
                     tab[archivoActual - 1][capacidadTamanioActual - tamanioArchivoActual][cantLineasActual - lineasArchivoActual];
                         int valorNoUsarArchivo = //para que se vea. 
                        tab[archivoActual - 1] [capacidadTamanioActual] [cantLineasActual];  
                        tab[archivoActual] [capacidadTamanioActual][cantLineasActual] = max(valorUsarArchivo, valorNoUsarArchivo); 
                    }
                 }
             }
             
        }
   
   return tab[cantArchivos][tamanio][cantLineas]; 
    }



int main()
{  
   int N; 
   cin >> N; 
   int S; 
   cin >> S; 
   int lineas; 
   cin >> lineas; 
   int* puntos = new int[N](); 
   int* arrLineas = new int[N](); //duda espacio. 
   int* tamanios = new int [N](); //error ñ. 
    for(int i = 0; i<N; i++) {
      int t, l , p ; 
      cin >> t >> l >> p; 
      puntos[i] = p; 
      tamanios[i] = t; 
      arrLineas[i] = l ; 
    }

  int res = ultimoMomento(N, S, lineas, tamanios, puntos, arrLineas); 
  cout << res << endl; 
}

