#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <climits>
using namespace std;

bool podemosPodar(int paso, int mejorPaso)
{
    return paso > mejorPaso;
}

bool esSolucion(int f, int c, char **mat, char elem)
{
    return mat[f][c] == elem; // encontre el elemento.
}
bool esMejorPaso(int paso, int mejorPaso)
{
    return paso < mejorPaso;
}

bool coordenadaValida(int f, int c, char **mat, int cantFilas, int cantColumnas)
{
    return f >= 0 && c >= 0 && f < cantFilas && c < cantColumnas;
}

bool puedoAplicarMovimiento(int fila, int col, bool **matB, char **mat, int cantFilas, int cantCol, char elem)
{
    if (coordenadaValida(fila, col, mat, cantFilas, cantCol) && !matB[fila][col]) {
    return mat[fila][col] == 'C' || mat[fila][col] == elem; 
    }else {
        return false; 
    }

};

void aplicarMovimiento(int fila, int col, bool **matB)
{
    matB[fila][col] = true;
};
void deshacerMovimiento(int fila, int col, bool **matB)
{
    matB[fila][col] = false; // deshacer es que vuelva a como estaba.
};

void mat_opt(char **mat, bool **matB, int filaAc, int colAc, int pasos, int &mejorPaso, char elem,
             int cantFilas, int cantCol)
{
    if (!podemosPodar(pasos, mejorPaso))  
    {
        if (esSolucion(filaAc, colAc, mat, elem) && esMejorPaso(pasos, mejorPaso))
        {
            mejorPaso = pasos;
        }
        else
        {
            int movF[4] = {1, 0, 0, -1}; // abajo, der, izq ,arriba.
            int movC[4] = {0, 1, -1, 0};
            for (int i = 0; i < 4; i++)
            {
                int nuevaFila = filaAc + movF[i];
                int nuevaCol = colAc + movC[i];
                if (puedoAplicarMovimiento(nuevaFila, nuevaCol, matB, mat, cantFilas, cantCol, elem))
                {
                    aplicarMovimiento(nuevaFila, nuevaCol, matB);
                    mat_opt(mat, matB, nuevaFila, nuevaCol, pasos + 1, mejorPaso, elem, cantFilas, cantCol);
                    deshacerMovimiento(nuevaFila, nuevaCol, matB);
                }
            }
        }
    }
};

int main()
{
    int res = INT_MAX;
    int mejorP = 0; // que me pase el mejor FC.
    char elem;
    cin >> elem;
    int cantMatrices;
    cin >> cantMatrices;

    for (int p = 1; p <= cantMatrices; p++)
    {
        int mejorSol = res; 
        int cantFilas, cantCol;
        cin >> cantFilas >> cantCol;

        char **mat = new char *[cantFilas];
        bool **matB = new bool *[cantFilas];
        for (int i = 0; i < cantFilas; i++)
        {
            mat[i] = new char[cantCol];
            matB[i] = new bool[cantCol];
            for (int j = 0; j < cantCol; j++)
            {
                char pos;
                cin >> pos;
                mat[i][j] = pos;
                matB[i][j] = false;
            }
            // llamo a todas que lo resuelvan , si es menor a mejorSol, actualizo res = p + " " + mejorSol.
        }
        matB[0][0] = true;
        mat_opt(mat, matB, 0, 0, 0, mejorSol, elem, cantFilas, cantCol);
        if (mejorSol < res)
        {
            res = mejorSol;
            mejorP = p;
        }
    };
    cout << mejorP << " " << res << endl;
}