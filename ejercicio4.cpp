#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct nodoLista
{
    int valor;
    nodoLista *sig;
};

class minHeap
{
private:
    nodoLista **arr;
    int proxLibre;
    int capacidad;

    int padre(int pos)
    {
        return pos / 2;
    }
    int hIzq(int pos)
    {
        return pos * 2;
    }
    int hDer(int pos)
    {
        return (pos * 2) + 1;
    }

    void swap(int pos1, int pos2)
    {
        nodoLista *aux = arr[pos1];
        arr[pos1] = arr[pos2];
        arr[pos2] = aux;
    }
    void flotar(int pos)
    {
        if (pos > 1)
        {
            int posPadre = padre(pos);
            if ((arr[pos]->valor < arr[posPadre]->valor))
            {
                swap(pos, posPadre);
                flotar(posPadre);
            }
        }
    }

    void hundir(int pos)
    {
        int posIzq = hIzq(pos);
        int posDer = hDer(pos);

        if (posIzq < proxLibre)
        {
            int posHMenor = posIzq;

            if (posDer < proxLibre && arr[posDer]->valor < arr[posIzq]->valor)
            {
                posHMenor = posDer;
            }
            if (arr[posHMenor]->valor < arr[pos]->valor)
            {
                swap(pos, posHMenor);
                hundir(posHMenor);
            }
        }
    }
    void removerTope()
    {
        assert(!estaVacio());
        proxLibre--;
        arr[1] = arr[proxLibre];
        hundir(1);
    }
    bool estaLleno()
    {
        return proxLibre > capacidad;
    }

    bool estaVacio()
    {
        return proxLibre == 1;
    }

    nodoLista *tope()
    {
        assert(!estaVacio());
        return arr[1];
    }

    void agregar(nodoLista *el)
    {
        assert(!estaLleno());
        arr[proxLibre] = el;
        proxLibre++;
        flotar(proxLibre - 1);
    }

public:
    minHeap(int _capacidad)
    {
        arr = new nodoLista *[_capacidad + 1]();
        proxLibre = 1;
        capacidad = _capacidad;
    }

    void fusionarListas(nodoLista **listas, int k)
    {
        for (int i = 0; i < k; i++)
        {
            if (listas[i] != NULL)
            {
                agregar(listas[i]);
            }
        }
        while (!estaVacio())
        {
            nodoLista *min = tope();
            cout << min->valor << endl;
            removerTope();
            if (min->sig != NULL)
            {
                agregar(min->sig);
            }
        }
    }
};

int main()
{
    int K;
    cin >> K;
    nodoLista **listas = new nodoLista *[K];
    for (int i = 0; i < K; i++)
    {
        int lim;
        cin >> lim;
        nodoLista *cabeza = NULL;
        nodoLista *cola = NULL;
        for (int j = 0; j < lim; j++)
        {
            nodoLista *nodo = new nodoLista();
            int dato;
            cin >> dato;
            nodo->valor = dato;
            nodo->sig = NULL;
            if (cabeza == NULL)
            {
                cabeza = cola = nodo;
            }
            else
            {
                cola->sig = nodo;
                cola = nodo;
            }
        }
        listas[i] = cabeza;
    };
    minHeap heap(K);
    heap.fusionarListas(listas, K);
}