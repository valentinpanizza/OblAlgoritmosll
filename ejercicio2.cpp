#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoLista {
    string path;
    string titulo;
    int tiempo;
    NodoLista* sig;
    NodoLista(string _path, string _titulo, int _tiempo) : path(_path), titulo(_titulo), tiempo(_tiempo), sig(NULL) {}
};

struct NodoHash {
    string dominio;
    NodoLista* l;
    bool activo;    
    NodoHash() : dominio(""), l(NULL), activo(false) {}
    NodoHash(string _dominio, NodoLista* _l) : dominio(_dominio), l(_l), activo(true) {}
};

class HashCerrado{
    private:
    NodoHash* arr;
    int B;
    int N;

    int obtenerPos(string _clave, int i){
        int h1 = hash1(_clave);
        int h2 = hash2(_clave);
        int pos = normalizar(h1 + i * (h2 + 1));
        if(!this->arr[pos].activo || this->arr[pos].dominio == _clave) return pos; // pos libre o mismo dom
        return obtenerPos(_clave, i + 1);
    }

    void insertar(string _clave, NodoLista* el){
        int pos = obtenerPos(_clave, 0);
        if(arr[pos].activo){
            el->sig = arr[pos].l;
            arr[pos].l = el;
        }else{
            arr[pos].activo = true;
            arr[pos].dominio = _clave;
            arr[pos].l = el;
            this->N++;
        }
    }

    int hash1(string key) {
        int h = 0;
        for (int i = 0; i < key.length(); i++)
            h = 31 * h + int(key[i]);
        return h;
    }

    int hash2(string key) {
        int h = 0;
        for (int i = 0; i < key.length(); i++)
            h = h + key[i] * (i + 1);
        return h;
    }

    int normalizar(int a){
        return abs(a) % B;
    }

    public:
    HashCerrado(int capacidad){
        B = capacidad;
        N = 0;
        arr = new NodoHash[B]();
    }

    void put(string dominio, string path, string titulo, int tiempo){
        if(N == B) return;
        NodoLista* nuevo = new NodoLista(path, titulo, tiempo);
        insertar(dominio, nuevo);
    }

    float factorDeCarga() {
        return (float)this->N/(float)this->B;
    }
};

int main()
{
    // TODO
    return 0;
}