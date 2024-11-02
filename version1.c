//En esta versión cada hilo mantiene su propio counter, en lugar de usar una variable global compartida. Al final, el programa suma los valores individuales de counter de cada hilo para obtener el total correcto.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max; // Variable global que indica cuántas veces cada hilo debe incrementar su contador

void *mythread(void *arg) {
    int *counter = (int *)arg; // Puntero a un entero, que apunta al contador propio de cada hilo
    int i; // Variable local que es privada para cada hilo
    printf("Thread: begin [addr of counter: %p]\n", counter);
    
    for (i = 0; i < max; i++) {
        (*counter)++; // Incrementa el contador del hilo correspondiente
    }
    printf("Thread: done\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: main-first <loopcount>\n");
        exit(1);
    }

    max = atoi(argv[1]);
    // Variables locales para almacenar el contador de cada hilo
    int counter1 = 0, counter2 = 0;
    // Identificadores de hilo
    pthread_t p1, p2; 

     // Imprimir estado inicial del contador
    printf("main: begin [counter1 = %d, counter2 = %d] [addr of counter1: %p, addr of counter2: %p]\n", counter1, counter2, &counter1, &counter2);

    pthread_create(&p1, NULL, mythread, &counter1);
    pthread_create(&p2, NULL, mythread, &counter2);

    // Espera a que ambos hilos terminen
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

     // Imprimir el valor de counter1 y counter2 antes del total
    printf("Final values: [counter1: %d, counter2: %d]\n", counter1, counter2);

    // Suma los contadores individuales para obtener el total
    int total = counter1 + counter2;
    printf("main: done\n [total counter: %d]\n [should: %d]\n", total, max * 2);
    return 0;
}
