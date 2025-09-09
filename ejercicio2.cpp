#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoHash {
    string dominio;
    string path;
    string titulo;
    int tiempo;
    bool activo;
    NodoHash(string _dominio, string _path, string _titulo, int _tiempo) : dominio(_dominio), path(_path), titulo(_titulo), tiempo(_tiempo), activo(true) {}
    NodoHash() : dominio(""), path(""), titulo(""), tiempo(0), activo(false) {}
};

class HashCerrado{
    private:
    NodoHash* arr;
    int B;
    int N;

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
};

int main()
{
    // TODO
    return 0;
}