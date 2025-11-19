#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoLista {
    string path; //search //images
    string titulo;
    int tiempo;
    NodoLista* sig;
    NodoLista(string _path, string _titulo, int _tiempo) : path(_path), titulo(_titulo), tiempo(_tiempo), sig(NULL) {}
};

struct NodoPath {
    NodoLista* dato;
    string dominio;
    bool activo;
    bool borrado;
    NodoPath() : dato(NULL), dominio(""), activo(false), borrado(false) {}
    NodoPath(string _dominio, NodoLista* l) : dato(l), dominio(_dominio), activo(true), borrado(false) {}
};

//tabla secundaria.
struct NodoHash {
    string dominio;
    int cant;
    NodoLista* l; //recibe nodoLista también. 
    bool activo; 
    bool borrado;   //necesito estos bool para la busqueda para que no se corte por ejemplo.
    NodoHash() : dominio(""), cant(0), l(NULL), activo(false), borrado(false) {}
    NodoHash(string _dominio, NodoLista* _l) : dominio(_dominio), cant(0), l(_l), activo(true), borrado(false) {}
};

class HashCerrado{
    private:
    // tabla de clave dom y valor lista de path
    NodoHash* arr; //tabla principal. 
    int B;
    int N;
    // tabla de clave dom + path y valor nodo de datos
    NodoPath* arrPath;

    bool esPrimo(int n) { 
        if (n <= 1) 
            return false; 
            for (int i = 2; i < n; i++) {
                if (n % i == 0) {
                    return false;
                }
            }
        return true;
    }

    int siguientePrimo (int n){
        int cont = n;
        while (true){
            if (esPrimo(cont)) return cont;
            cont++;
        }
    }

    int obtenerPos(string _clave) {
        int h1 = abs(hash1(_clave)) % B;
        int h2 = 1 + (abs(hash2(_clave)) % (B - 1));
        int borrado = -1;
        for (int i = 0; i < B; ++i) {
            int pos = (h1 + i * h2) % B;
            // si no hubo borrado sirve para insertar
            if (!arr[pos].activo && !arr[pos].borrado) {
                return (borrado != -1) ? borrado : pos;
            }
            // guardar borrado
            if (arr[pos].activo && arr[pos].borrado) {
                if (borrado == -1) borrado = pos;
                continue;
            }
            // comparar dominio
            if (arr[pos].dominio == _clave) {
                return pos;
            }
        }
        // si hay borrado insertar
        return (borrado != -1) ? borrado : -1;
    }

    int obtenerPosPath(string dominio, string path) {
        string key = dominio + path;
        int h1 = abs(hash1(key)) % B;
        int h2 = 1 + (abs(hash2(key)) % (B - 1));
        int borrado = -1;
        for (int i = 0; i < B; ++i) {
            int pos = (h1 + i * h2) % B;
            if (!arrPath[pos].activo && !arrPath[pos].borrado) {
                return (borrado != -1) ? borrado : pos;
            }
            if (arrPath[pos].activo && arrPath[pos].borrado) {
                if (borrado == -1) borrado = pos;
                continue;
            }
            if (arrPath[pos].dominio == dominio &&
                arrPath[pos].dato != NULL &&
                arrPath[pos].dato->path == path) {
                return pos;
            }
        }
        return (borrado != -1) ? borrado : -1;
    }

    bool existe(string dominio, string path){
        int pos = obtenerPosPath(dominio, path);
        if(pos == -1) return false;
        if(!arrPath[pos].activo) return false;
        // por las dudas
        return (arrPath[pos].dominio == dominio && arrPath[pos].dato && arrPath[pos].dato->path == path);
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

    void liberarLista(NodoLista* raiz) {
        while (raiz) {
            NodoLista* aux = raiz->sig;
            delete raiz;
            raiz = aux;
        }
    }

    public:
    HashCerrado(int capacidad){
        B = siguientePrimo(capacidad*2); // doble para menos colisiones
        N = 0;
        arr = new NodoHash[B]();
        arrPath = new NodoPath[B]();
    }

    void put(string dominio, string path, string titulo, int tiempo){
        int pos = obtenerPos(dominio);
        if (pos == -1) return;
        if (!arr[pos].activo || arr[pos].borrado) {
            arr[pos].dominio = dominio;
            arr[pos].l = NULL;
            arr[pos].cant = 0;
            arr[pos].activo = true;
            arr[pos].borrado = false;
        }
        // buscar path
        int posPath = obtenerPosPath(dominio, path);
        if (posPath != -1 && arrPath[posPath].activo && !arrPath[posPath].borrado &&
            arrPath[posPath].dato != NULL && arrPath[posPath].dato->path == path) {
            // existe
            NodoLista* node = arrPath[posPath].dato;
            node->titulo = titulo;
            node->tiempo = tiempo;
            // mover al frente de la lista
            if (arr[pos].l != node) {
                NodoLista* prev = nullptr;
                NodoLista* cur  = arr[pos].l;
                while (cur && cur != node) { prev = cur; cur = cur->sig; }
                if (cur) { // encontrado en la lista
                    if (prev) prev->sig = cur->sig;   // lo saco de su lugar
                    cur->sig = arr[pos].l;            // lo mando al frente
                    arr[pos].l = cur;
                }
            }
            return;
        }
        // no existe
        NodoLista* nuevo = new NodoLista(path, titulo, tiempo);
        nuevo->sig = arr[pos].l;
        arr[pos].l = nuevo;
        arr[pos].cant++;
        this->N++;
        posPath = obtenerPosPath(dominio, path);
        if (posPath == -1) return;
        arrPath[posPath].dominio = dominio;
        arrPath[posPath].dato = nuevo;
        arrPath[posPath].activo = true;
        arrPath[posPath].borrado = false;
    }

    string get(string dominio, string path){
        int pos = obtenerPosPath(dominio, path);
        if (pos == -1) return "recurso_no_encontrado";
        if (!arrPath[pos].activo || arrPath[pos].borrado) return "recurso_no_encontrado";
        if (arrPath[pos].dominio != dominio || arrPath[pos].dato == nullptr || arrPath[pos].dato->path != path) {
            return "recurso_no_encontrado";
        }
        return arrPath[pos].dato->titulo + " " + to_string(arrPath[pos].dato->tiempo);
    }

    string contains(string dominio, string path) {
        return existe(dominio, path) ? "true" : "false";
    }

    void clear_domain(const string& dominio) {
        int pos = obtenerPos(dominio);
        if (pos == -1 || !arr[pos].activo || arr[pos].borrado) return;
        NodoLista* aux = arr[pos].l;
        while (aux) {
            int pr = obtenerPosPath(dominio, aux->path);
            if (pr != -1 && arrPath[pr].activo && !arrPath[pr].borrado && 
                arrPath[pr].dato && arrPath[pr].dato->path == aux->path) {
                arrPath[pr].borrado = true;
                arrPath[pr].activo  = true;
                arrPath[pr].dato    = NULL;
            }
            NodoLista* borro = aux;
            aux = aux->sig;
            delete borro;
            --N;
        }
        // vaciar lista del dominio
        arr[pos].l = NULL;
        arr[pos].cant = 0;
        arr[pos].borrado = true;
        arr[pos].activo  = true;
    }

    int size() {
        return N;
    }

    void clear() {
        for (int i = 0; i < B; ++i) {
            if (arr[i].l) {
                liberarLista(arr[i].l);
                arr[i].l = NULL;
            }
            arr[i].dominio = "";
            arr[i].cant    = 0;
            arr[i].activo  = false;   
            arr[i].borrado = false;   
        }
        for (int j = 0; j < B; ++j) {
            arrPath[j].dato    = NULL;
            arrPath[j].dominio = "";
            arrPath[j].activo  = false;
            arrPath[j].borrado = false; 
        }
        N = 0;
    }

    void remove(string dominio, string path) {
        int posDom = obtenerPos(dominio); //pos en tabla principal.
        if (posDom == -1 || !arr[posDom].activo || arr[posDom].borrado) return;
        int posPath = obtenerPosPath(dominio, path); //la busco en la tabla secundaria.
        if (posPath == -1 || !arrPath[posPath].activo || arrPath[posPath].borrado ||
            arrPath[posPath].dato == NULL || arrPath[posPath].dato->path != path) {
            return;
        }
        NodoLista* nodoP = arrPath[posPath].dato;
        NodoLista* aux = NULL;
        NodoLista* nodoD = arr[posDom].l;
        while (nodoD && nodoD != nodoP) { 
            aux = nodoD; 
            nodoD = nodoD->sig; 
        }
        if (!nodoD) return;
        if (aux) aux->sig = nodoD->sig;
        else      arr[posDom].l = nodoD->sig;

        delete nodoD;
        --N;
        --arr[posDom].cant;
        arrPath[posPath].borrado = true;
        arrPath[posPath].activo = true;
        arrPath[posPath].dato = NULL;
    }

    int count_domain(string dominio){
        int pos = obtenerPos(dominio);
        if(pos == -1 || !arr[pos].activo) return 0;
        return arr[pos].cant;
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