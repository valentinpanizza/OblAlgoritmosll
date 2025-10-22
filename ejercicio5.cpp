#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/ListImp.cpp"
using namespace std;

struct Arista {
    int origen;
    int destino;
    int peso;

    Arista() : origen(0), destino(0), peso(0) {}
    Arista(int o, int d, int p) : origen(o), destino(d), peso(p) {}
};

class minHeapp {
private:
    Arista** array;
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
        Arista* aux = array[pos1];
        array[pos1] = array[pos2];
        array[pos2] = aux;
    }

    void flotar(int i) {
        if (i > 1) {
            if (array[i]->peso < array[i / 2]->peso) {
                swap(i, i / 2);
            }
            flotar(i / 2);
        }
    }

    void hundir(int p) {
        if (Hizq(p) < proxaColocar) {
            int hMenorpos = Hizq(p);
            if (Hder(p) < proxaColocar) {
                if (array[Hder(p)]->peso < array[Hizq(p)]->peso) {
                    hMenorpos = Hder(p);
                }
            }
            if (array[p]->peso > array[hMenorpos]->peso) {
                swap(p, hMenorpos);
                hundir(hMenorpos);
            }
        }
    }

    bool estaLleno() {
        return proxaColocar > N;
    }

public:
    minHeapp(int n) {
        array = new Arista*[n + 1];
        proxaColocar = 1;
        N = n;
    }

    void agregar(Arista* nuevo) {
        if (estaLleno()) return;
        array[proxaColocar] = nuevo;
        flotar(proxaColocar);
        proxaColocar++;
    }

    Arista* removerTope() {
        if (!estaVacio()) {
            Arista* a = array[1];
            array[1] = array[proxaColocar - 1]; 
            proxaColocar--;
            hundir(1);
            return a;
        }
        return NULL;
    }

    Arista* tope() {
        if (!estaVacio())
            return array[1];
        return NULL;
    }

    bool estaVacio() {
        return proxaColocar == 1;
    }
};

class mfSet {
private:
    int* padres;
    int* altura;
    int cant;

public:
    mfSet(int cantidad) {
        padres = new int[cantidad];
        altura = new int[cantidad];
        cant = cantidad;

        for (int i = 0; i < cantidad; i++) {
            padres[i] = -1;
            altura[i] = 1;
        }
    }

    int find(int v) {
        if (padres[v] == v || padres[v] == -1) {
            return v;
        } else {
            padres[v] = find(padres[v]);
            return padres[v];
        }
    }

    void unir(int v1, int v2) {
        int padrev1 = find(v1);
        int padrev2 = find(v2);

        if (padrev1 == padrev2) return;

        if (altura[padrev1] > altura[padrev2]) {
            padres[padrev2] = padrev1;
        } else if (altura[padrev1] < altura[padrev2]) {
            padres[padrev1] = padrev2;
        } else {
            padres[padrev1] = padrev2;
            altura[padrev2]++;
        }
    }
};

int main() {
    int V, K;
    cin >> V >> K;

    int cont = 0;
    int peso = 0;

    minHeapp heap(K);

    for (int i = 0; i < K; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        Arista* a = new Arista(u, v, w);
        heap.agregar(a);
    }

    mfSet mf(V);

    while (!heap.estaVacio()) {
        Arista* a = heap.removerTope();
        if (mf.find(a->origen) != mf.find(a->destino)) {
            mf.unir(a->origen, a->destino);
            cont++;
            peso += a->peso;
        }
        delete a; // opcional
    }

    cout << peso << endl;
    return 0;
}