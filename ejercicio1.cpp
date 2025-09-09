#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoAVL {
    int id;
    int puntaje;
    string nombre;
    int alturaId;
    int alturaP;
    int cantNodos; // en el AVL de puntaje
    NodoAVL* izqId;
    NodoAVL* derId;
    NodoAVL* izqP;
    NodoAVL* derP;
    NodoAVL(int _id, int _puntaje, string _nombre) : id(_id), puntaje(_puntaje), nombre(_nombre), alturaId(1), alturaP(1), cantNodos(1), izqId(NULL), derId(NULL), izqP(NULL), derP(NULL) {}
};

class AVL {

    private:
    NodoAVL* raizId;
    NodoAVL* raizP;
    int cant;
    NodoAVL* mejor;

    int alturaSegura(NodoAVL* nodo, bool id){
        if(!nodo) return 0;
        if(id) return nodo->alturaId;
        return nodo->alturaP;
    }

    int getBalance(NodoAVL* nodo, bool id){
        if(!nodo) return 0;
        if(id) return alturaSegura(nodo->derId, true) - alturaSegura(nodo->izqId, true);
        return alturaSegura(nodo->derP, false) - alturaSegura(nodo->izqP, false);
    }

    void actualizarAltura(NodoAVL* nodo){
        nodo->alturaId = 1 + max(alturaSegura(nodo->derId, true), alturaSegura(nodo->izqId, true));
        nodo->alturaP = 1 + max(alturaSegura(nodo->derP, false), alturaSegura(nodo->izqP, false));
    }

    void rotacionHaciaIzquierda(NodoAVL*& A, bool id){
        if (id){
            NodoAVL* B = A->derId;
            NodoAVL* T2 = B->izqId;
            A->derId = T2;
            B->izqId = A;
            actualizarAltura(A);
            actualizarHijos(A);
            actualizarAltura(B);
            actualizarHijos(B);
            A = B;
        }else{
            NodoAVL* B = A->derP;
            NodoAVL* T2 = B->izqP;
            A->derP = T2;
            B->izqP = A;
            actualizarAltura(A);
            actualizarHijos(A);
            actualizarAltura(B);
            actualizarHijos(B);
            A = B;
        }
    }

    void rotacionHaciaDerecha(NodoAVL*& B, bool id){
        if(id){
            NodoAVL* A = B->izqId;
            NodoAVL* T2 = A->derId;
            B->izqId = T2;
            A->derId = B;
            actualizarAltura(B);
            actualizarHijos(B);
            actualizarAltura(A);
            actualizarHijos(A);
            B = A;
        }else{
            NodoAVL* A = B->izqP;
            NodoAVL* T2 = A->derP;
            B->izqP = T2;
            A->derP = B;
            actualizarAltura(B);
            actualizarHijos(B);
            actualizarAltura(A);
            actualizarHijos(A);
            B = A;
        }
    }

    void insertarDos(int id, int puntaje, string nombre) {
        NodoAVL* nuevo = new NodoAVL(id, puntaje, nombre);
        this->cant = this->cant + 1;
        if (this->mejor == NULL || puntaje > this->mejor->puntaje || (puntaje == this->mejor->puntaje && id < this->mejor->id)) {
            this->mejor = nuevo;
        }
        insertarRecId(this->raizId, nuevo);  // inserta en árbol de IDs
        insertarRecP(this->raizP, nuevo);    // inserta en árbol de Puntajes
    }

    void insertarRecId(NodoAVL*& nodo, NodoAVL* nuevo){
        if(!nodo){
            nodo = nuevo;
            return;
        }else if(nodo->id < nuevo->id) {
            insertarRecId(nodo->derId, nuevo);
        }else if(nodo->id > nuevo->id) {
            insertarRecId(nodo->izqId, nuevo);
        }

        nodo->alturaId = 1 + max(alturaSegura(nodo->derId, true), alturaSegura(nodo->izqId, true));

        int balance = getBalance(nodo, true);

        // caso izq izq
        if(balance < -1 && nodo->izqId->id > nuevo->id){
            rotacionHaciaDerecha(nodo, true);
        // caso izq der
        }else if(balance < -1 && nodo->izqId->id < nuevo->id){
            rotacionHaciaIzquierda(nodo->izqId, true);
            rotacionHaciaDerecha(nodo, true);
        } else{
        // caso der der
            if(balance > 1 && nodo->derId->id < nuevo->id){
                rotacionHaciaIzquierda(nodo, true);
        // caso der izq
            }else if(balance > 1 && nodo->derId->id > nuevo->id){
                rotacionHaciaDerecha(nodo->derId, true);
                rotacionHaciaIzquierda(nodo, true);
            }
        }
    }

    void insertarRecP(NodoAVL*& nodo, NodoAVL* nuevo){
        if(!nodo){
            nodo = nuevo;
            return;
        }else if(nodo->puntaje < nuevo->puntaje) {
            insertarRecP(nodo->derP, nuevo);
        }else if(nodo->puntaje > nuevo->puntaje) {
            insertarRecP(nodo->izqP, nuevo);
        }else if(nodo->puntaje == nuevo->puntaje) {
            if (nuevo->id < nodo->id){
                insertarRecP(nodo->izqP, nuevo);
            }else{
                insertarRecP(nodo->derP, nuevo);
            }
        }

        nodo->alturaP = 1 + max(alturaSegura(nodo->derP, false), alturaSegura(nodo->izqP, false));

        int balance = getBalance(nodo, false);

        // caso izq izq
        if(balance < -1 && nodo->izqP->puntaje > nuevo->puntaje){
            rotacionHaciaDerecha(nodo, false);
        // caso izq der
        }else if(balance < -1 && nodo->izqP->puntaje < nuevo->puntaje){
            rotacionHaciaIzquierda(nodo->izqP, false);
            rotacionHaciaDerecha(nodo, false);
        } else{
        // caso der der
            if(balance > 1 && nodo->derP->puntaje < nuevo->puntaje){
                rotacionHaciaIzquierda(nodo, false);
        // caso der izq
            }else if(balance > 1 && nodo->derP->puntaje > nuevo->puntaje){
                rotacionHaciaDerecha(nodo->derP, false);
                rotacionHaciaIzquierda(nodo, false);
            }
        }
        actualizarHijos(nodo);
    }

    NodoAVL* buscarId(NodoAVL* nodo, int id){
        if(!nodo) return NULL;
        if(nodo->id == id) return nodo;
        if(id < nodo->id) return buscarId(nodo->izqId, id);
        return buscarId(nodo->derId, id);
    }

    int contPuntaje(NodoAVL* nodo, int p) {
        if (!nodo) return 0;
        if (nodo->puntaje < p) {
            return contPuntaje(nodo->derP, p);
        } else {
            if(nodo->derP) return 1 + nodo->derP->cantNodos + contPuntaje(nodo->izqP, p);
            return 1 + contPuntaje(nodo->izqP, p);
        }
    }

    void actualizarHijos(NodoAVL* nodo) {
        if (!nodo) return;
        int izq = nodo->izqP ? nodo->izqP->cantNodos : 0;
        int der = nodo->derP ? nodo->derP->cantNodos : 0;
        nodo->cantNodos = 1 + izq + der;
    }

    public:
    AVL() {
        raizId = NULL;
        raizP = NULL;
        cant = 0;
        mejor = NULL;
    }

    void insertar(int id, int puntaje, string nombre){
        if (buscarId(this->raizId, id) != NULL) return;
        this->insertarDos(id, puntaje, nombre);
    }

    bool esVacio(){
        return this->raizId == NULL;
    }

    string impFind(int _id){
        NodoAVL* n = buscarId(this->raizId, _id);
        return (n == NULL) ? "jugador_no_encontrado" : n->nombre + " " + std::to_string(n->puntaje);
    }

    string impRank(int p){
        return std::to_string(contPuntaje(this->raizP, p));
    }

    string getCant(){
        return std::to_string(this->cant);
    }

    string impTop1(){
        if(!this->mejor) return "sin_jugadores";
        return this->mejor->nombre + " " + std::to_string(this->mejor->puntaje);
    }
};

int main()
{
    int cant;
    std::cin >> cant;
    AVL avl;
    for (int i = 0; i < cant; ++i) {
        string op;
        cin >> op;
        if (op == "ADD") {
            int id, puntaje;
            string nombre;
            cin >> id >> nombre >> puntaje;
            avl.insertar(id, puntaje, nombre);
        } else if (op == "FIND") {
            int id;
            cin >> id;
            cout << avl.impFind(id) << endl;
        } else if (op == "RANK") {
            int p;
            cin >> p;
            cout << avl.impRank(p) << endl;
        } else if (op == "TOP1") {
            cout << avl.impTop1() << endl;
        } else if (op == "COUNT") {
            cout << avl.getCant() << endl;
        }
    }
}