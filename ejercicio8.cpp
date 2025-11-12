#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;


int maxPuntos_rec(int* colores, int*** tab, int i, int f, int k){
    if(f < i) return 0;
    if(i == f) return ((k+1)*(k+1));
    if(tab[i][f][k] == -1){
        int uso = (k+1)*(k+1) + maxPuntos_rec(colores, tab, i+1, f, 0);
        int maximo = 0;
        for(int m = i + 1; m <= f; m++){
            if(colores[i] == colores[m]){
                int actual = maxPuntos_rec(colores, tab, i+1, m-1, 0) + maxPuntos_rec(colores, tab, m, f, k+1);
                if(actual > maximo) maximo = actual;
            }
        }
        tab[i][f][k] = max(uso, maximo);
    }
    return tab[i][f][k];
}

int maxPuntos(int cantElementos, int* colores){
    int *** tab = new int**[cantElementos];
    for(int i = 0; i < cantElementos; i++){
        tab[i] = new int*[cantElementos];
        for(int j = 0; j < cantElementos; j++){
            tab[i][j] = new int[cantElementos];
            for(int k = 0; k < cantElementos; k++){
                tab[i][j][k] = -1;
            }
        }
    }

    return maxPuntos_rec(colores, tab, 0, cantElementos - 1, 0);
}


int main()
{
    int cantElementos;
    cin >> cantElementos;

    int* colores = new int[cantElementos];

    for(int i = 0; i < cantElementos; i++){
        cin >> colores[i];
    }

    cout << maxPuntos(cantElementos, colores) << endl;
}