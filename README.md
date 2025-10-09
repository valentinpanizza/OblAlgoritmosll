# Obligatorio 1 - Estructuras de datos y Algoritmos II
###### Valentin Panizza - 302616 -   Mateo Bartaburu - 

### Ejercicio 1
Nos basamos en el codigo visto en clase para realizar las operaciones de:
```
void rotacionHaciaIzquierda(NodoAVL*& A, bool id)
void rotacionHaciaDerecha(NodoAVL*& B, bool id)
void insertarRecId(NodoAVL*& nodo, NodoAVL* nuevo)
void insertarRecP(NodoAVL*& nodo, NodoAVL* nuevo)
```

### Ejercicio 2
Utilizamos dos funciones de hash vistas en clase:
```
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
```

### Ejercicio 3
**Comentar acerca de funcion de hash! (o cambiarla)**

### Ejercicio 4
**Mateo comentar**

### Ejercicio 5
Nos basamos en las interpretaciones de MFset y MinHeap vistas en clase para realizar el ejercicio.