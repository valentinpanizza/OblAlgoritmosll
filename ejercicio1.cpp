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
    NodoAVL* izqId;
    NodoAVL* derId;
    NodoAVL* izqP;
    NodoAVL* derP;
    NodoAVL(int _id, int _puntaje, string _nombre) : id(_id), puntaje(_puntaje), nombre(_nombre), alturaId(1), alturaP(1), izqId(NULL), derId(NULL), izqP(NULL), derP(NULL) {}
};

class AVL {

    private:
    NodoAVL* raizId;
    NodoAVL* raizP;
    int cant;
    int mejorP;

    void inOrderRecId(NodoAVL* nodo){
        if(nodo){
            inOrderRecId(nodo->izqId);
            std::cout << nodo->id << endl;
            inOrderRecId(nodo->derId);
        }
    }

    void inOrderRecP(NodoAVL* nodo){
        if(nodo){
            inOrderRecP(nodo->izqP);
            std::cout << nodo->puntaje << endl;
            inOrderRecP(nodo->derP);
        }
    }

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
            actualizarAltura(B);
            A = B;
        }else{
            NodoAVL* B = A->derP;
            NodoAVL* T2 = B->izqP;
            A->derP = T2;
            B->izqP = A;
            actualizarAltura(A);
            actualizarAltura(B);
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
            actualizarAltura(A);
            B = A;
        }else{
            NodoAVL* A = B->izqP;
            NodoAVL* T2 = A->derP;
            B->izqP = T2;
            A->derP = B;
            actualizarAltura(B);
            actualizarAltura(A);
            B = A;
        }
    }

    void insertarDos(int id, int puntaje, string nombre) {
        NodoAVL* nuevo = new NodoAVL(id, puntaje, nombre);
        this->cant++;
        if(puntaje > this->mejorP) mejorP = puntaje;
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
    }

    NodoAVL* buscarId(NodoAVL* nodo, int id){
        if(!nodo) return NULL;
        if(nodo->id == id) return nodo;
        if(id < nodo->id) return buscarId(nodo->izqId, id);
        return buscarId(nodo->derId, id);
    }

    int contPuntaje(NodoAVL* nodo, int p) {
        if(!nodo) return 0;
        if(nodo->puntaje > p) {
            return 1 + contPuntaje(nodo->izqP, p) + contPuntaje(nodo->derP, p);
        } else {
            return contPuntaje(nodo->derP, p);
        }
    }

    public:
    AVL() {
        raizId = NULL;
        raizP = NULL;
    }

    void insertar(int id, int puntaje, string nombre){
        this->insertarDos(id, puntaje, nombre);
    }

    void inOrderId(){
        this->inOrderRecId(this->raizId);
    }

    void inOrderP(){
        this->inOrderRecP(this->raizP);
    }

    bool esVacio(){
        return this->raizId == NULL;
    }

    string impFind(int _id){
        NodoAVL* n = buscarId(this->raizId, _id);
        return (n == NULL) ? "jugador no encontrado" : n->nombre + " " + std::to_string(n->puntaje);
    }

    string impRank(int p){
        return std::to_string(contPuntaje(this->raizP, p));
    }
};

int main()
{
    return 0;
}