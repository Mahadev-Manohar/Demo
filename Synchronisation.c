#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function
#include <time.h>   // for time function

sem_t empty, full;
pthread_mutex_t mutex;
int count = 0;
int buffer[10];

void* producer(void* args) {
    int* num = ((int*)args); // Dereference the argument properly
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[count] = rand() % 100;
    printf("Producer %d produced %d\n", *num + 1, buffer[count]);
    count++;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    sleep(1); // Add sleep to ensure the thread doesn't exit immediately
    pthread_exit(NULL);
}

void* consumer(void* args) {
    int* num = ((int*)args); // Dereference the argument properly
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    count--;
    printf("Consumer %d consumed %d\n", *num + 1, buffer[count]);
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    sleep(1); // Add sleep to ensure the thread doesn't exit immediately
   pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    int numberOfProducers, numberOfConsumers;
    printf("Enter the number of producers: ");
    scanf("%d", &numberOfProducers);
    printf("Enter the number of consumers: ");
    scanf("%d", &numberOfConsumers);

    sem_init(&empty, 0, 10);
    sem_init(&full, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    pthread_t prod[numberOfProducers], cons[numberOfConsumers];
    unsigned long int i;

    int *prod_args = malloc(sizeof(int) * numberOfProducers);
    int *cons_args = malloc(sizeof(int) * numberOfConsumers);

    for (i = 0; i < numberOfProducers; i++) {
        prod_args[i] = i; // Assign thread index as argument
        pthread_create(&prod[i], NULL, producer, (void*)&prod_args[i]); // Pass address of argument
    }
    for (i = 0; i < numberOfConsumers; i++) {
        cons_args[i] = i; // Assign thread index as argument
        pthread_create(&cons[i], NULL, consumer, (void*)&cons_args[i]); // Pass address of argument
     }

    for (i = 0; i < numberOfProducers; i++) {
        pthread_join(prod[i], NULL);
    }
    for (i = 0; i < numberOfConsumers; i++) {
        pthread_join(cons[i], NULL);
    }

    free(prod_args);
    free(cons_args);

    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
    return 0;
}
