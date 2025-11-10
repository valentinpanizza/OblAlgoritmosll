# Obligatorio 1 - Estructuras de datos y Algoritmos II
#### Valentin Panizza - 302616 -   Mateo Bartaburu - 326771


<br>
<br>
<br>
<br>
<p align="center">
  <img src="img/ORT.jpg" alt="Descripción" width="1200">
</p>
<br>
<br>
<br>
<br>
<p align="center">
  <img src="img/valentin.png" alt="Imagen 1" width="400">
  <img src="img/mateo.png" alt="Imagen 2" width="400">
</p>

## Ejercicio 1

**Explicación breve de la solución:**
Se implementó un AVL doble, uno ordenado por id (para búsquedas rápidas) y otro por puntaje (para ranking). Cada inserción agrega el mismo nodo en ambos AVL. Se mantiene además un puntero al mejor jugador y un contador total.

**Justificación de la complejidad:**
Gracias al uso de árboles AVL, todas las operaciones principales (ADD, FIND, RANK) se realizan en O(log n). TOP1 y COUNT son O(1) al usar referencias y contadores.

Nos basamos en el codigo visto en clase para realizar las operaciones de:
```
void rotacionHaciaIzquierda(NodoAVL*& A, bool id)
void rotacionHaciaDerecha(NodoAVL*& B, bool id)
void insertarRecId(NodoAVL*& nodo, NodoAVL* nuevo)
void insertarRecP(NodoAVL*& nodo, NodoAVL* nuevo)
```

## Ejercicio 2

**Explicación breve de la solución:**
Se usa hash cerrado con doble hashing en dos tablas:

1. arr: clave = dominio → valor = lista enlazada de paths del dominio (con contador por dominio).
2. arrPath: clave = (dominio+path) → valor = puntero al nodo en la lista (para acceso directo).
   Operaciones: PUT inserta/actualiza (y mueve al frente en la lista), GET/CONTAINS consultan vía arrPath, REMOVE borra de la lista y marca borrado en arrPath, CLEAR_DOMAIN barre la lista del dominio y marca borrados, LIST_DOMAIN recorre la lista, COUNT_DOMAIN y SIZE usan contadores.

**Justificación de la complejidad:**
* Estructuras: tablas hash cerradas con doble hashing + listas enlazadas por dominio + punteros cruzados.
* PUT nuevo: O(1); PUT existente (mover al frente): O(k) (desenlazar en lista del dominio).
* GET, CONTAINS: O(1) esperado (por arrPath).
* REMOVE: O(k) (desenlazar en lista del dominio).
* CLEAR_DOMAIN: O(k) (recorre la lista del dominio).
* LIST_DOMAIN: O(k); COUNT_DOMAIN, SIZE: O(1).

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

## Ejercicio 3

**Explicación breve de la solución:**
Se implementa un hash abierto donde cada bucket encadena nodos de dominio (NodoHash). Cada dominio mantiene una lista enlazada de recursos (NodoLista) con sus path, titulo y tiempo.
Operaciones: PUT inserta/actualiza y, si el path ya existe, lo mueve al comienzo; GET/CONTAINS buscan dentro de la lista del dominio; REMOVE quita un path; CLEAR_DOMAIN elimina toda la lista del dominio; LIST_DOMAIN recorre la lista; SIZE y factorDeCarga usan contadores.

**Justificación de la complejidad:**
* PUT: O(1 + k) (hash + cadena promedio del bucket, y búsqueda en lista del dominio de tamaño k). Inserción nueva es O(1).
* GET, CONTAINS: O(1 + k) (buscar dominio en bucket + recorrer lista del dominio).
* REMOVE: O(1 + k).
* CLEAR_DOMAIN: O(k).
* LIST_DOMAIN: O(k).
* COUNT_DOMAIN: O(k).
* SIZE: O(1).

## Ejercicio 4

**Explicación breve de la solución:**
Se fusionan k listas enlazadas ordenadas usando un min-heap que guarda punteros a los cabezales de cada lista. Se extrae siempre el nodo con menor valor, se imprime y, si tenía siguiente, se vuelve a insertar ese siguiente en el heap.

**Justificación de la complejidad:**
* Estructuras: listas enlazadas simples + min-heap de tamaño ≤ k.
* Construcción inicial del heap con hasta k elementos: O(k log k).
* Por cada uno de los N nodos: extracción e inserción en el heap → O(log k).
* Total: O(N log k), espacio adicional O(k) por el heap.

Nos basamos en las interpretaciones de MinHeap vistas en clase para realizar el ejercicio. 

## Ejercicio 5

**Explicación breve de la solución:**
Se implementa Kruskal para obtener el peso del MST:
* Las aristas se priorizan en un MinHeap(comparador por peso).
* Se usa MFset (path compression + union by rank) para evitar ciclos.
* Se extrae la arista mínima; si conecta dos componentes distintas, se une en MFset y se suma su peso. Se detiene al tomar v-1 aristas.

**Justificación de la complejidad:**
* Construcción del heap por inserciones: O(E log E) (cada agregar al MinHeap cuesta log E).
* Bucle principal: hasta E extracciones tope/remover del MinHeap → O(E log E), y cada find/merge del MFset es O(log V).
* Total: O(E log E + V log V) = (apox) O((V+E)logV).
* Espacio: O(E + V) (heap de aristas + estructuras del MFset).

## Pruebas

| Problema  | Resultado      |
|-----------|----------------|
| 1         | Completo       |
| 2         | Completo       |
| 3         | Completo       |
| 4         | Completo       |
| 5         | Completo       |
