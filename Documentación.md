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


**Justificación de la complejidad:**


## Ejercicio 7

**Explicación breve de la solución:**

**Justificación de la complejidad:**

## Ejercicio 8

**Explicación breve de la solución:**

**Justificación de la complejidad:**

## Ejercicio 9

**Explicación breve de la solución:**

**Justificación de la complejidad:**

## Ejercicio 10

**Explicación breve de la solución:**
Se implementó backtracking con poda, utilizamos una matriz de booleanos "matrizB" que evita recorrer 2 veces la misma celda y de esta manera evita ciclos. 
Para la poda realizamos 2 cosas: mediante la función de podemosPodar que descarta recorridos que requieras de mas pasos que el mejor encontrado, y además dentro de main asignando a mejorSol = res,  lo que permite descartar recorridos que requieran más pasos que la mejor solución global.
Además se sigue la estructura tipica de backtracking, luego de verificar que el movimiento es válido,  realizamos el "sanguche" de aplicarMovimiento, la llamada recursiva a mat_opt y deshacerMovimiento. 
Desde main, se realiza el backtracking para cada FC, seleccionando el que requiera de menor cantidad de pasos.


**Justificación de la complejidad:**

## Pruebas

| Problema  | Resultado      |
|-----------|----------------|
| 6         | Completo       |
| 7         | Completo       |
| 8         | Completo       |
| 9         | Completo       |
| 10        | Completo       |
