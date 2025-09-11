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

    int obtenerPos(string _clave) {
        int h1 = hash1(_clave);
        int h2 = hash2(_clave);
        for (int i = 0; i < B; ++i) {
            int pos = normalizar(h1 + i * h2);
            if (!arr[pos].activo || arr[pos].dominio == _clave)
                return pos;
        }
        return -1; // tabla llena
    }

    bool existe(string clave, string path){
        int pos = obtenerPos(clave);
        if(pos == -1) return false;
        if(!arr[pos].activo) return false;
        NodoLista* aux = arr[pos].l;
        while(aux){
            if(aux->path == path)
                return true;
            aux = aux->sig;
        }
        return false;
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
        int pos = obtenerPos(dominio);
        if(pos == -1) return;
        if(!arr[pos].activo){ 
            arr[pos].dominio = dominio;
            arr[pos].l = NULL;
            arr[pos].activo = true;
        }
        NodoLista* aux = arr[pos].l;
        NodoLista* aux2 = NULL;
        while(aux){
            if(aux->path == path){
                aux->titulo = titulo;
                aux->tiempo = tiempo;
                if(aux2){ // no esta al principio
                    aux2->sig = aux->sig;
                    aux->sig = arr[pos].l;
                    arr[pos].l = aux;
                }
                return;
            }
            aux2 = aux;
            aux = aux->sig;
        }
        NodoLista* nuevo = new NodoLista(path, titulo, tiempo);
        nuevo->sig = arr[pos].l;
        arr[pos].l = nuevo;
        this->N++;
    }

    float factorDeCarga() {
        return (float)this->N/(float)this->B;
    }

    string get(string dominio, string path){
        int pos = obtenerPos(dominio);
        if(pos == -1 || !arr[pos].activo) return "recurso_no_encontrado";
        NodoLista* aux = arr[pos].l;
        while(aux){
            if(aux->path == path)
                return aux->titulo + " " + to_string(aux->tiempo);
            aux = aux->sig;
        }
        return "recurso_no_encontrado";
    }

    string contains(string dominio, string path) {
        return existe(dominio, path) ? "true" : "false";
    }

    void clear_domain(string dominio) {
        int pos = obtenerPos(dominio);
        if(pos == -1 || !arr[pos].activo) return;
        NodoLista* aux = arr[pos].l;
        while(aux){
            NodoLista* borro = aux;
            aux = aux->sig;
            delete borro;
            this->N--;
        }
        arr[pos].l = NULL;
        arr[pos].activo = false;
        arr[pos].dominio = "";
    }

    int size() {
        return N;
    }

    void clear() {
        for(int i = 0; i < B; ++i){
            NodoLista* aux = arr[i].l;
            while(aux){
                NodoLista* borro = aux;
                aux = aux->sig;
                delete borro;
            }
            arr[i].l = NULL;
            arr[i].activo = false;
            arr[i].dominio = "";
        }
        N = 0;
    }

    void remove(string dominio, string path) {
        int pos = obtenerPos(dominio);
        if(pos == -1 || !arr[pos].activo) return;
        NodoLista* aux = arr[pos].l;
        NodoLista* prev = NULL;
        while(aux){
            if(aux->path == path){
                if(prev) prev->sig = aux->sig;
                else arr[pos].l = aux->sig;
                delete aux;
                this->N--;
                return;
            }
            prev = aux;
            aux = aux->sig;
        }
    }

    int count_domain(string dominio){
        int pos = obtenerPos(dominio);
        if(pos == -1 || !arr[pos].activo) return 0;
        int count = 0;
        NodoLista* aux = arr[pos].l;
        while(aux){
            count++;
            aux = aux->sig;
        }
        return count;
    }

    string list_domain(string dominio){
        int pos = obtenerPos(dominio);
        if(pos == -1 || !arr[pos].activo) return "";
        string ret = "";
        NodoLista* aux = arr[pos].l;
        while(aux){
            ret += aux->path ;
            aux = aux->sig;
            if(aux) ret += " ";
        }
        return ret;
    }
};

int main()
{
    int cant;
    std::cin >> cant;
    HashCerrado* tabla = new HashCerrado(cant);
    for (int i = 0; i < cant; ++i) {
        string op;
        cin >> op;
        if (op == "PUT") {
            int tiempo;
            string dominio, path, titulo;
            cin >> dominio >> path >> titulo >> tiempo;
            tabla->put(dominio, path, titulo, tiempo);
        } else if (op == "GET") {
            string dominio, path;
            cin >> dominio >> path;
            cout << tabla->get(dominio, path) << endl;
        } else if (op == "REMOVE") {
            string dominio, path;
            cin >> dominio >> path;
            tabla->remove(dominio, path);
        } else if (op == "CONTAINS") {
            string dominio, path;
            cin >> dominio >> path;
            cout << tabla->contains(dominio, path) << endl;
        } else if (op == "COUNT_DOMAIN") {
            string dominio;
            cin >> dominio;
            cout << tabla->count_domain(dominio) << endl;
        } else if (op == "LIST_DOMAIN"){
            string dominio;
            cin >> dominio;
            cout << tabla->list_domain(dominio) << endl;
        } else if (op == "CLEAR_DOMAIN"){
            string dominio;
            cin >> dominio;
            tabla->clear_domain(dominio);
        } else if (op == "SIZE"){
            cout << tabla->size() << endl;
        } else if (op == "CLEAR"){
            tabla->clear();
        }
    }
}