#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Arista
{
    int origen;
    int destino;
    int peso;
    Arista() : origen(0), destino(0), peso(0) {}
    Arista(int o, int d, int p) : origen(o), destino(d), peso(p) {}
};

bool esMenor(Arista a1, Arista a2){
    return a1.peso < a2.peso;
}

class MFset
{
private:
    int *grupo;
    int *altura;
    int cant;

public:
    MFset(int n)
    {
        cant = n;
        grupo = new int[n];
        altura = new int[n];
        for (int i = 0; i < n; i++)
        {
            grupo[i] = i;
            altura[i] = 0;
        }
    }

    int find(int x)
    {
        if (grupo[x] != x) grupo[x] = find(grupo[x]);
        return grupo[x];
    }

    void merge(int x, int y)
    {
        int grupoX = find(x);
        int grupoY = find(y);
        if (grupoX != grupoY)
        {
            if (altura[grupoX] < altura[grupoY])
            {
                grupo[grupoX] = grupoY;
            }
            else if (altura[grupoX] > altura[grupoY])
            {
                grupo[grupoY] = grupoX;
            }
            else
            {
                grupo[grupoY] = grupoX;
                altura[grupoX]++;
            }
        }
    }
};

class MinHeap
{
    private:
    Arista* arr;
    int proxLibre;
    int cap;
    bool (*esMenor)(Arista, Arista);

    int padre(int pos){
        return pos / 2;
    }

    int hIzq(int pos){
        return pos * 2;
    }

    int hDer(int pos) {
        return (pos * 2) + 1;
    }

    void swap(int pos1, int pos2){
        Arista a = arr[pos1];
        arr[pos1] = arr[pos2];
        arr[pos2] = a;
    }

    void flotar(int pos){
        if(pos > 1){ // no estoy en la raiz
            int posPadre = padre(pos);
            if(esMenor(arr[pos], arr[posPadre])){
                swap(pos, posPadre);
                flotar(posPadre);
            }
        }
    }

    void hundir(int pos){
        int posIzq = hIzq(pos);
        int posDer = hDer(pos);
        if(posIzq < proxLibre) { // no es una hoja
            int posHMenor = posIzq;
            if(posDer < proxLibre && esMenor(arr[posDer], arr[posIzq])) { // si el derecho es menor
                posHMenor = posDer; // cambio el menor
            }
            if(esMenor(arr[posHMenor], arr[pos])) {
                swap(pos, posHMenor);
                hundir(posHMenor);
            }
        }
    }

    public:
    MinHeap(int _cap, bool (*_esMenor)(Arista, Arista)){
        arr = new Arista[_cap + 1];
        proxLibre = 1;
        cap = _cap;
        esMenor = _esMenor;
    }

    void agregar(Arista el){
        assert(!estaLleno());
        arr[proxLibre] = el;
        flotar(proxLibre);
        proxLibre++;
    }

    void removerTope(){
        assert(!estaVacio());
        arr[1] = arr[proxLibre-1];
        proxLibre--;
        hundir(1);
    }

    bool estaLleno(){
        return proxLibre > cap; // >= para base 0
    }

    bool estaVacio(){
        return proxLibre == 1; // 0 para base 0
    }

    Arista tope(){
        assert(!estaVacio());
        return arr[1];
    }
};

int main()
{
    int v, e;
    int peso = 0;
    int cant = 0;
    cin >> v >> e;
    MinHeap* aristas = new MinHeap(e, esMenor);
    int o, d, p;
    for(int i = 0; i < e; i++){
        cin >> o >> d >> p;
        aristas->agregar(Arista(o, d, p));
    }
    MFset* mfset = new MFset(v);
    while(!aristas->estaVacio() ){
        Arista a = aristas->tope();
        aristas->removerTope();

        if(mfset->find(a.origen) != mfset->find(a.destino)){
            mfset->merge(a.origen, a.destino);
            peso += a.peso;
            cant++;
            if(cant == v - 1) break;
        }
    }
    cout << peso << endl;
    delete mfset;
    delete aristas;
    return 0;
}