#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

struct NodoLista {
    string path;
    string titulo;
    int tiempo;
    NodoLista* sigP;
    NodoLista(string _path, string _titulo, int _tiempo)
        : path(_path), titulo(_titulo), tiempo(_tiempo), sigP(NULL) {}
};

struct NodoHash {
    string dominio;
    NodoLista* l; // cabeza de la lista de recursos del dominio
    NodoHash* sig; // siguiente dominio en el bucket
    NodoHash() : dominio(""), l(NULL), sig(NULL) {}
    NodoHash(string _dominio, NodoLista* _l) : dominio(_dominio), l(_l), sig(NULL) {}
};

class HashAbierto {
private:
    NodoHash** arr;    // buckets
    int B;             // tamaño tabla
    int N;             // cantidad total de recursos (NO cuenta dominios)

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
        return abs(a) % B;
    }

    // Inserta/actualiza dentro del bucket encadenado
    void insertarRecD(NodoHash*& nodo, string _clave, NodoLista* _valor){
        if(!nodo){
            // nuevo dominio con primer recurso
            nodo = new NodoHash(_clave, _valor);
            N++; // agregamos un recurso nuevo
            return;
        }
        if (nodo->dominio == _clave){
            // buscar si el path ya existe
            NodoLista* cur  = nodo->l;
            NodoLista* prev = NULL;
            while(cur){
                if(cur->path == _valor->path){
                    // actualizar y mover a cabeza si no está ya
                    cur->titulo = _valor->titulo;
                    cur->tiempo = _valor->tiempo;
                    if(prev){
                        prev->sigP = cur->sigP;
                        cur->sigP  = nodo->l;
                        nodo->l    = cur;
                    }
                    delete _valor; // no lo necesitamos, era temporal
                    return;
                }
                prev = cur;
                cur  = cur->sigP;
            }
            // no existía: insertar al frente (más reciente)
            _valor->sigP = nodo->l;
            nodo->l      = _valor;
            N++; // recurso nuevo
            return;
        }
        insertarRecD(nodo->sig, _clave, _valor);
    }

    bool existeRec(NodoHash* nodo, string _clave){
        if(!nodo) return false;
        return (nodo->dominio == _clave) ? true : existeRec(nodo->sig, _clave);
    }

    NodoLista* consultarRec(NodoHash* nodo, string _clave){
        if(!nodo) return NULL;
        return (nodo->dominio == _clave) ? nodo->l : consultarRec(nodo->sig, _clave);
    }

    int hash(string key) {
        long long h = 0;
        for (size_t i = 0; i < key.length(); i++)
            h = h * 131 + (unsigned char)key[i];
        // compactar a int
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

    void insertarD(string _clave, NodoLista* _valor) {
        int pos = normalizar(hash(_clave));
        insertarRecD(arr[pos], _clave, _valor);
    }

    bool existe(string _clave){
        int pos = normalizar(hash(_clave));
        return existeRec(arr[pos], _clave);
    }

    NodoLista* consultar(string clave) {
        assert(existe(clave));
        int pos = normalizar(hash(clave));
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
                    aux2 = aux2->sigP;
                    delete borro2;
                }
                delete borro;
            }
            arr[i] = NULL;
        }
        N = 0;
    }

    void put(string dominio, string path, string titulo, int tiempo){
        NodoLista* nuevo = new NodoLista(path, titulo, tiempo);
        insertarD(dominio, nuevo);
    }

    string get(string dominio, string path){
        if(!existe(dominio)) return "recurso_no_encontrado";
        NodoLista* aux = consultar(dominio);
        while(aux){
            if(aux->path == path)
                return aux->titulo + " " + to_string(aux->tiempo);
            aux = aux->sigP; 
        }
        return "recurso_no_encontrado";
    }

    string contains(string dominio, string path) {
        if(!existe(dominio)) return "false";
        NodoLista* aux = consultar(dominio);
        while(aux){
            if(aux->path == path)
                return "true";
            aux = aux->sigP;
        }
        return "false";
    }

    void clear_domain(string dominio) {
        if(!existe(dominio)) return;
        int pos = normalizar(hash(dominio));
        NodoHash* aux = arr[pos];
        NodoHash* prev = NULL;
        while(aux){
            if(aux->dominio == dominio){
                // sacar nodo dominio de la lista del bucket
                if(prev) prev->sig = aux->sig;
                else arr[pos] = aux->sig;

                // liberar todos los recursos y descontar N
                NodoLista* cur = aux->l;
                while(cur){
                    NodoLista* borro = cur;
                    cur = cur->sigP;
                    delete borro;
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
            aux = aux->sigP; 
        }
        return count;
    }

    string list_domain(string dominio){
        if(!existe(dominio)) return "";
        string ret = "";
        NodoLista* aux = consultar(dominio);
        while(aux){
            ret += aux->path ;
            aux = aux->sigP;
            if(aux) ret += " ";
        }
        return ret;
    }

    void remove(string dominio, string path) {
        if(!existe(dominio)) return;
        int pos = normalizar(hash(dominio));
        NodoHash* aux = arr[pos];
        while(aux){
            if(aux->dominio == dominio){
                NodoLista* cur = aux->l;
                NodoLista* prev = NULL;
                while(cur){
                    if(cur->path == path){
                        if(prev) prev->sigP = cur->sigP;
                        else aux->l = cur->sigP;
                        delete cur;
                        this->N--;
                        return;
                    }
                    prev = cur;
                    cur = cur->sigP;
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
    if(!(cin >> cant)) return 0;
    HashAbierto* tabla = new HashAbierto(max(1, cant));

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
            cout << tabla->get(dominio, path) << "\n";
        } else if (op == "REMOVE") {
            string dominio, path;
            cin >> dominio >> path;
            tabla->remove(dominio, path);
        } else if (op == "CONTAINS") {
            string dominio, path;
            cin >> dominio >> path;
            cout << tabla->contains(dominio, path) << "\n";
        } else if (op == "COUNT_DOMAIN") {
            string dominio;
            cin >> dominio;
            cout << tabla->count_domain(dominio) << "\n";
        } else if (op == "LIST_DOMAIN"){
            string dominio;
            cin >> dominio;
            cout << tabla->list_domain(dominio) << "\n";
        } else if (op == "CLEAR_DOMAIN"){
            string dominio;
            cin >> dominio;
            tabla->clear_domain(dominio);
        } else if (op == "SIZE"){
            cout << tabla->size() << "\n";
        } else if (op == "CLEAR"){
            tabla->clear();
        }
    }
    delete tabla;
    return 0;
}