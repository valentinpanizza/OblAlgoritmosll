# Obligatorio 2 - Estructuras de datos y Algoritmos II
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

## Ejercicio 6

**Explicación breve de la solución:**
Se utilizó un max heap para seleccionar siempre el vecino con mayor potencia disponible.
La idea es recorrer el camino hasta el destino y, cada vez que aparece un nuevo vecino, agregar su potencia al heap.
Cuando se llega al inicio de un pozo, se calcula la potencia necesaria para atravesarlo (largo del pozo + 2).
Si la potencia actual no alcanza, se obtiene del heap el vecino con mayor potencia, sumando su potencia y aumentando el contador de vecinos utilizados.
Esto se repite hasta cruzar el pozo o determinar que no es posible (heap vacío).
Luego se salta hasta el final del pozo y se continúa el recorrido.

**Justificación de la complejidad:**
N = cantidad de pozos
M = cantidad de mejoras/vecinos disponibles
En total no puede haber más de M inserciones ni más de M extracciones.
O(Nlog M)
ya que M <= N

Espacio: 
O(N+M)
Se guardan N pozos en un arreglo.
Se almacenan hasta M mejoras en el heap.

## Ejercicio 7

**Explicación breve de la solución:**
Se guarda cada clave del primer conjunto en una tabla hash, donde la clave es la palabra y el valor es su posición.
Luego se lee el segundo conjunto de claves y, por cada una, se consulta su posición original.
Se genera así un arreglo con la permutación.
Sobre ese arreglo se aplica Merge Sort modificado, que además de ordenar:
Cuenta cuántos elementos de la mitad izquierda son mayores que uno de la mitad derecha.
Estas son exactamente las inversiones.
Se usa la función intercalar() para sumar n1 - i cada vez que un elemento de la derecha es más chico que uno de la izquierda.

**Justificación de la complejidad:**
Inserción y consulta en hash abierto: O(1) promedio, O(n) en total.
MergeSort modificado: O(nlog n).

Utilizamos una función de hash vista en clase:
```
int hash(string key) {
    int h = 0;
    for (int i = 0; i < key.length(); i++)
        h = 31 * h + int(key[i]);
      return h;
}
```

## Ejercicio 8

**Explicación breve de la solución:**
La solución usa programación dinámica con memoización:
tab[i][f][k] que representa el máximo puntaje posible en el intervalo [i,f] si ya existen k elementos del mismo color que colores[i] acumulados a la izquierda.

El caso recursivo:
Usar colores[i] ahora, sumando (k+1)^2 y resolver [i+1,f].
Buscar una posición m donde colores[m] == colores[i] y juntar bloques, resolviendo primero lo que quede en el medio [i+1,m−1], para luego juntar y aumentar k.
El máximo entre esas opciones es la solución.

**Justificación de la complejidad:**
La tabla tab tiene tamaño O(n^3)
Y para cada pos se realiza una recorrida de búsqueda de coincidencias O(n)
Por lo tanto, la complejidad es O(n^4)

## Ejercicio 9

**Explicación breve de la solución:**
Se usa PD con tabulación:
tab[i][t][l] que indica el máximo puntaje posible utilizando:
Los primeros i archivos,
Con tamaño total <= t,
Con líneas totales <= l.

No tomar el archivo:
tab[i][t][l]=tab[i−1][t][l]

Tomar el archivo si entra en tamaño y líneas:
tab[i][t][l]=puntos[i]+tab[i−1][t−tamanio][l−lineas]

El resultado final es tab[cantidad][tamanio][linea].

**Justificación de la complejidad:**
i: hasta cantidad
t: hasta tamanio
l: hasta linea
O(cantidad.tamanio.linea)

## Ejercicio 10

**Explicación breve de la solución:**
Se implementó backtracking con poda, utilizamos una matriz de booleanos "matrizB" que evita recorrer 2 veces la misma celda y de esta manera evita ciclos. 
Para la poda realizamos 2 cosas: mediante la función de podemosPodar que descarta recorridos que requieras de mas pasos que el mejor encontrado, y además dentro de main asignando a mejorSol = res,  lo que permite descartar recorridos que requieran más pasos que la mejor solución global.
Además se sigue la estructura tipica de backtracking, luego de verificar que el movimiento es válido,  realizamos el "sanguche" de aplicarMovimiento, la llamada recursiva a mat_opt y deshacerMovimiento. 
Desde main, se realiza el backtracking para cada FC, seleccionando el que requiera de menor cantidad de pasos.

## Pruebas

| Problema  | Resultado      |
|-----------|----------------|
| 6         | Completo       |
| 7         | Completo       |
| 8         | Completo       |
| 9         | Completo       |
| 10        | Completo       |