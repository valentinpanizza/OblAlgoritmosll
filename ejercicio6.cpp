#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Pozo{
    int inicio;
    int final;
    Pozo() : inicio(), final() {}
    Pozo(int inicio, int final) : inicio(inicio), final(final)  {}
};

class maxHeapp {
private:
    int* array;
    int N;
    int proxaColocar;

    int padre(int i) { 
        return i / 2;
     }
    int Hizq(int i) {
         return i * 2; 
        }
    int Hder(int i) {
         return i * 2 + 1; 
        }

    void swap(int pos1, int pos2) {
        int aux = array[pos1];
        array[pos1] = array[pos2];
        array[pos2] = aux;
    }

    void flotar(int i) {
        if (i > 1) {
            if (array[i] > array[i / 2]) {
                swap(i, i / 2);
            }
            flotar(i / 2);
        }
    }

    void hundir(int p) {
        if (Hizq(p) < proxaColocar) {
            int hMayorpos = Hizq(p);
            if (Hder(p) < proxaColocar) {
                if (array[Hder(p)] > array[Hizq(p)]){
                    hMayorpos = Hder(p);
                }
            }
            if (array[p] < array[hMayorpos]) {
                swap(p, hMayorpos);
                hundir(hMayorpos);
            }
        }
    }

    bool estaLleno() {
        return proxaColocar > N;
    }

public:
    maxHeapp(int n) {
        array = new int[n + 1];
        proxaColocar = 1;
        N = n;
    }

    void agregar(int nuevo) {
        if (estaLleno()) return;
        array[proxaColocar] = nuevo;
        flotar(proxaColocar);
        proxaColocar++;
    }

    int removerTope() {
        if (!estaVacio()) {
            int a = array[1];
            array[1] = array[proxaColocar - 1]; 
            proxaColocar--;
            hundir(1);
            return a;
        }
        return 0;
    }

    int tope() {
        if (!estaVacio())
            return array[1];
        return 0;
    }

    bool estaVacio() {
        return proxaColocar == 1;
    }
};


int main()
{
    int ret = 0;
    int posPozo = 0;

    int cantPozos, cantVecinos, destino;
    int inicioPozo, finalPozo;
    int vecino = 0, potencia = 0;

    int P = 1;

    cin >> cantPozos >> cantVecinos >> destino;

    maxHeapp* vecinos = new maxHeapp(cantVecinos);
    Pozo* pozos = new Pozo[cantPozos];

    for(int i = 0; i < cantPozos; i++){
        cin >> inicioPozo >> finalPozo;
        pozos[i] = Pozo(inicioPozo, finalPozo);
    }

    if (cantVecinos > 0) {
        cin >> vecino >> potencia;
        cantVecinos--;
    } else {
        vecino = destino + 1; // para no agregar en 0
    }

    for(int i = 1; i <= destino; i++){
        while(vecino <= i){
            vecinos->agregar(potencia);
            vecino = destino + 1; // para no volver a agregar este vecino
            if(cantVecinos > 0) {
                cin >> vecino >> potencia;
                cantVecinos--;
            } else {
                break; // no hay mas vecinos
            }
        }

        if (i + 1 == pozos[posPozo].inicio){
            int potenciaNecesaria = (pozos[posPozo].final - pozos[posPozo].inicio) + 2; // largo del pozo mas un margen en inicio y final
            while(P < potenciaNecesaria){
                if(vecinos->estaVacio()){
                    cout << "Imposible" << endl;
                    return 0;
                } else {
                    P += vecinos->removerTope();
                    ret++;
                }
            }
            //P = P - potenciaNecesaria; (pense q se gastaba la potencia pero no)
            if(--cantPozos == 0) break;
            posPozo++;
            i = pozos[posPozo - 1].final - 1; // -1 porque despues del for se incrementa
        }
    }

    cout << ret << endl;
    return 0;
}



   
