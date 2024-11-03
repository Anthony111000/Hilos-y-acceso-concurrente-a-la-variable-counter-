// En esta versión el número de hilos(variable) y el valor de max se pasan como argumentos. Cada hilo tiene su propio counter, y el programa los suma al final para obtener el resultado correcto.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max; // Cantidad de incrementos que cada hilo debe realizar
int num_threads; // Número de hilos a crear, especificado por el usuario

void *mythread(void *arg) {
    int *counter = (int *)arg; // Puntero al contador de este hilo
    int i;

    // Impresión al inicio del hilo
    printf("Thread: begin [addr of counter: %p]\n", counter);
    // Cada hilo incrementa su propio contador "max" veces
    for (i = 0; i < max; i++) {
        (*counter)++;
    }
    printf("Thread: done\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    // Verifica que se hayan pasado dos argumentos: max y num_threads
    if (argc != 3) {
        fprintf(stderr, "usage: main <loopcount> <num_threads>\n");
        exit(1);
    }

    max = atoi(argv[1]); // Primer argumento
    num_threads = atoi(argv[2]); // Segundo argumento

    // Reserva memoria para almacenar los contadores e identificadores de cada hilo
    int *counters = malloc(num_threads * sizeof(int));
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));

    // Inicialización de los contadores en 0
    for (int i = 0; i < num_threads; i++) {
        counters[i] = 0;
    }

    // Estado inicial de todos los contadores y sus direcciones de memoria
    printf("main: begin [");
    for (int i = 0; i < num_threads; i++) {
        printf("counter%d = %d", i + 1, counters[i]);
        if (i < num_threads - 1) {
            printf(", "); // Separar contadores con una coma
        }
    }
    printf("] ");
    for (int i = 0; i < num_threads; i++) {
        printf("[addr of counter%d: %p] ", i + 1, &counters[i]); // Mostrar dirección de cada contador
    }
    printf("\n");

    // Crear los hilos, cada uno con su propio contador
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, mythread, &counters[i]);
    }

    // join espera a que todos los hilos terminen
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL); 
    }

    // Imprimir los valores finales de cada contador
    printf("Final values: [");
    for (int i = 0; i < num_threads; i++) {
        printf("counter%d: %d", i + 1, counters[i]);
        if (i < num_threads - 1) {
            printf(", "); // Separar contadores con una coma
        }
    }
    printf("]\n");

    // Calcular el total de todos los contadores
    long long total = 0; // Usar long long para evitar desbordamiento en la suma
    for (int i = 0; i < num_threads; i++) {
        total += counters[i];
    }

    // Imprimir el total acumulado
    printf("main: done\n [total counter: %lld]\n [should: %d]\n", total, max * num_threads);

    // Liberar memoria asignada para los contadores y hilos
    free(counters);
    free(threads);
    return 0;
}
