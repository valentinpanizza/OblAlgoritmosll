#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoHash {
    string clave;
    int valor;
    NodoHash* sig;
    NodoHash() : clave(""), valor(0), sig(NULL) {}
    NodoHash(string _clave, int _valor) : clave(_clave), valor(_valor), sig(NULL) {}
};

class HashAbierto {
private:
    NodoHash** arr;
    int B;
    int N;

    bool esPrimo(int n) {
        if (n < 2) return false;
        if (n % 2 == 0) return n == 2;
        for (int i = 3; 1LL*i*i <= n; i += 2)
            if (n % i == 0) return false;
        return true;
    }

    int siguientePrimo (int n){
        if (n <= 2) return 2;
        if (n % 2 == 0) n++;
        while (!esPrimo(n)) n += 2;
        return n;
    }

    int normalizar(int a){
        return (a < 0 ? -a : a) % B;
    }

    bool existeRec(NodoHash* nodo, string _clave){
        if(!nodo) return false;
        return (nodo->clave == _clave) ? true : existeRec(nodo->sig, _clave);
    }

    int consultarRec(NodoHash* nodo, string _clave){
        if(!nodo) return -1;
        return (nodo->clave == _clave) ? nodo->valor : consultarRec(nodo->sig, _clave);
    }

    int hash(string key) {
        unsigned long long h = 0;
        for (size_t i = 0; i < key.length(); i++)
            h = h * 131 + (unsigned char)key[i];
        // Comprimo a int
        return (int)(h ^ (h >> 32));
    }

public:
    HashAbierto(int capacidad){
        B = siguientePrimo(capacidad * 2);
        N = 0;
        arr = new NodoHash*[B]();
        for(int i = 0; i<B; i++) {
            arr[i] = NULL;
        }
    }

    ~HashAbierto(){
        clear();
        delete[] arr;
    }

    void insertar(string clave, int valor) {
        int pos = normalizar(hash(clave));
        for (NodoHash* p = arr[pos]; p; p = p->sig) {
            if (p->clave == clave) {
                p->valor = valor;
                return;                
            }
        }
        NodoHash* nuevo = new NodoHash(clave, valor);
        nuevo->sig = arr[pos];
        arr[pos] = nuevo;
        ++N;
    }

    bool existe(string _clave){
        int pos = normalizar(hash(_clave));
        return existeRec(arr[pos], _clave);
    }

    int consultar(string clave) {
        assert(existe(clave));
        int pos = normalizar(hash(clave));
        return consultarRec(arr[pos], clave);
    }

    float factorDeCarga() {
        return (float)this->N/(float)this->B;
    }

    int size() const {
        return N;
    }

    void clear() {
        for(int i = 0; i < B; ++i){
            NodoHash* aux = arr[i];
            while(aux){
                NodoHash* borro = aux;
                aux = aux->sig;
                delete borro;
            }
            arr[i] = NULL;
        }
        N = 0;
    }
};

int* intercalar(int arr1[], int n1, int arr2[], int n2, long& cont) {
    int* ret = new int[n1 + n2];

    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (arr1[i] <= arr2[j]) {
            ret[k++] = arr1[i++];
        } else {
            ret[k++] = arr2[j++];
            cont += long(n1 - i);   // todas las parejas
        }
    }

    while (i < n1) ret[k++] = arr1[i++];
    while (j < n2) ret[k++] = arr2[j++];

    return ret;
}

int* mergeSort(int valores[], int inicio, int fin, long& cont){
    if (inicio == fin) {
        int* base = new int[1];
        base[0] = valores[inicio];
        return base;
    }

    int mitad = (inicio + fin) / 2;

    int* ord1 = mergeSort(valores, inicio, mitad, cont);
    int n1 = mitad - inicio + 1;

    int* ord2 = mergeSort(valores, mitad + 1, fin, cont);
    int n2 = fin - mitad;

    return intercalar(ord1, n1, ord2, n2, cont);
}

int main() {
    int n;
    cin >> n;

    HashAbierto tabla(n);
    for (int i = 0; i < n; ++i) {
        string clave;
        cin >> clave;
        tabla.insertar(clave, i + 1);
    }

    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        string clave;
        cin >> clave;
        arr[i] = tabla.consultar(clave);
    }

    long cont = 0;
    int* ordenado = mergeSort(arr, 0, n - 1, cont);

    cout << cont;

    return 0;
}