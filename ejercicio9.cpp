#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int maximosPuntos(int cantidad, int tamanio, int linea, int* tamanios, int* lineas, int* puntos){
    int *** tab = new int**[cantidad + 1];
    for(int i = 0; i <= cantidad; i++){
        tab[i] = new int*[tamanio + 1];
        for(int j = 0; j <= tamanio; j++){
            tab[i][j] = new int[linea + 1]();
        }
    }

    for(int archivoActual = 1; archivoActual <= cantidad; archivoActual++){
        for(int tamActual = 1; tamActual <= tamanio; tamActual++){
            for(int lineaActual = 1; lineaActual <= linea; lineaActual++){
                if(tamanios[archivoActual - 1] > tamActual || lineas[archivoActual - 1] > lineaActual){ // Si se pasa de tamanio o de cantidad de lineas
                    tab[archivoActual][tamActual][lineaActual] = tab[archivoActual - 1][tamActual][lineaActual];
                } else {
                    tab[archivoActual][tamActual][lineaActual] = max(
                        puntos[archivoActual-1] + tab[archivoActual-1][tamActual - tamanios[archivoActual - 1]]
                        [lineaActual - lineas[archivoActual - 1]],
                        tab[archivoActual - 1][tamActual][lineaActual]);
                }
            }
        }
    }

    return tab[cantidad][tamanio][linea];
}

int main()
{
    int cantidad, tamanio, linea;
    cin >> cantidad;
    cin >> tamanio;
    cin >> linea;

    int* tamanios = new int[cantidad]();
    int* lineas = new int[cantidad]();
    int* puntos = new int[cantidad]();

    for(int i = 0; i < cantidad; i++){
        int t, l, p;
        cin >> t >> l >> p;
        tamanios[i] = t;
        lineas[i] = l;
        puntos[i] = p;
    }

    cout << maximosPuntos(cantidad, tamanio, linea, tamanios, lineas, puntos) << endl;
    return 0;
}