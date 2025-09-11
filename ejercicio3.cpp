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
    NodoHash* sig;
    NodoHash() : dominio(""), l(NULL), sig(NULL) {}
    NodoHash(string _dominio, NodoLista* _l) : dominio(_dominio), l(_l), sig(NULL) {}
};

class HashAbierto
{
    private:
    NodoHash** arr;
    int B;
    int N;

    int normalizar(int a){
        return abs(a) % B;
    }

    void insertarRec(NodoHash*& nodo, string _clave, NodoLista* _valor){
        if(!nodo){
            nodo = new NodoHash(_clave, _valor);
            N++;
        } else {
            if (nodo->dominio == _clave)
                nodo->l = _valor;
            else 
                insertarRec(nodo->sig, _clave, _valor);
        }
    }

    bool existeRec(NodoHash* nodo, string _clave){
        if(!nodo){
            return false;
        } else {
            return (nodo->dominio == _clave) ? true : existeRec(nodo->sig, _clave);
        }
    }

    NodoLista* consultarRec(NodoHash* nodo, string _clave){
        return (nodo->dominio == _clave) ? nodo->l : consultarRec(nodo->sig, _clave);
    }

    int hash(string key) {
        int h = 0;
        for (int i = 0; i < key.length(); i++)
            h = 31 * h + int(key[i]);
        return h;
    }

    public:
    HashAbierto(int capacidad){
        B = capacidad;
        arr = new NodoHash*[B]();
        N = 0;
        for(int i = 0; i<B; i++) {
            arr[i] = NULL;
        }
    }

    void insertar(string _clave, NodoLista* _valor) {
        int res = hash(_clave);
        int pos = normalizar(res);
        insertarRec(arr[pos], _clave, _valor);
    }

    bool existe(string _clave){
        int res = hash(_clave);
        int pos = normalizar(res);
        return existeRec(arr[pos], _clave);
    }

    NodoLista* consultar(string clave) {
        assert(existe(clave));
        int res = hash(clave); 
        int pos = normalizar(res);
        return consultarRec(arr[pos], clave);
    }

    float factorDeCarga() {
        return (float)this->N/(float)this->B;
    }

    int size() {
        return N;
    }

    void clear() {
        for(int i = 0; i < B; ++i){
            NodoHash* aux = arr[i];
            while(aux){
                NodoHash* borro = aux;
                aux = aux->sig;
                NodoLista* aux2 = borro->l;
                while(aux2){
                    NodoLista* borro2 = aux2;
                    aux2 = aux2->sig;
                    delete borro2;
                }
                delete borro;
            }
            arr[i] = NULL;
        }
        N = 0;
    }

    void put(string dominio, string path, string titulo, int tiempo){
        int res = hash(dominio);
        int pos = normalizar(res);
        if(!existe(dominio)){
            NodoLista* nuevo = new NodoLista(path, titulo, tiempo);
            insertar(dominio, nuevo);
            return;
        }
        NodoLista* aux = consultar(dominio);
        NodoLista* aux2 = NULL;
        while(aux){
            if(aux->path == path){
                aux->titulo = titulo;
                aux->tiempo = tiempo;
                if(aux2){ // no esta al principio
                    aux2->sig = aux->sig;
                    aux->sig = consultar(dominio);
                    insertar(dominio, aux); // lo pongo al principio
                }
                return;
            }
            aux2 = aux;
            aux = aux->sig;
        }
        NodoLista* nuevo = new NodoLista(path, titulo, tiempo);
        nuevo->sig = consultar(dominio);
        insertar(dominio, nuevo);
        this->N++;
    }

    string get(string dominio, string path){
        if(!existe(dominio)) return "recurso_no_encontrado";
        NodoLista* aux = consultar(dominio);
        while(aux){
            if(aux->path == path)
                return aux->titulo + " " + to_string(aux->tiempo);
            aux = aux->sig;
        }
        return "recurso_no_encontrado";
    }

    string contains(string dominio, string path) {
        if(!existe(dominio)) return "false";
        NodoLista* aux = consultar(dominio);
        while(aux){
            if(aux->path == path)
                return "true";
            aux = aux->sig;
        }
        return "false";
    }

    void clear_domain(string dominio) {
        if(!existe(dominio)) return;
        int res = hash(dominio);
        int pos = normalizar(res);
        NodoHash* aux = arr[pos];
        NodoHash* prev = NULL;
        while(aux){
            if(aux->dominio == dominio){
                if(prev) prev->sig = aux->sig;
                else arr[pos] = aux->sig;
                NodoLista* aux2 = aux->l;
                while(aux2){
                    NodoLista* borro2 = aux2;
                    aux2 = aux2->sig;
                    delete borro2;
                    this->N--;
                }
                delete aux;
                return;
            }
            prev = aux;
            aux = aux->sig;
        }
    }

    int count_domain(string dominio){
        if(!existe(dominio)) return 0;
        NodoLista* aux = consultar(dominio);
        int count = 0;
        while(aux){
            count++;
            aux = aux->sig;
        }
        return count;
    }

    string list_domain(string dominio){
        if(!existe(dominio)) return "";
        string ret = "";
        NodoLista* aux = consultar(dominio);
        while(aux){
            ret += aux->path ;
            aux = aux->sig;
            if(aux) ret += " ";
        }
        return ret;
    }

    void remove(string dominio, string path) {
        if(!existe(dominio)) return;
        int res = hash(dominio);
        int pos = normalizar(res);
        NodoHash* aux = arr[pos];
        while(aux){
            if(aux->dominio == dominio){
                NodoLista* aux2 = aux->l;
                NodoLista* prev = NULL;
                while(aux2){
                    if(aux2->path == path){
                        if(prev) prev->sig = aux2->sig;
                        else aux->l = aux2->sig;
                        delete aux2;
                        this->N--;
                        return;
                    }
                    prev = aux2;
                    aux2 = aux2->sig;
                }
                return;
            }
            aux = aux->sig;
        }
    }
};

int main()
{
    int cant;
    std::cin >> cant;
    HashAbierto* tabla = new HashAbierto(cant);
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