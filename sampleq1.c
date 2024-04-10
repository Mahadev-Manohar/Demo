
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5 // Number of diners the restaurant can hold at a time

// Semaphore declarations
sem_t entering_diners, exiting_diners, enter_done, exit_done;
sem_t mutex_enter, mutex_exit;

// Counter declarations
int count_enter = 0, count_exit = 0;

void *restaurant_thread(void *arg) {
    while (1) {
        // Open front door
        openFrontDoor();

        // Signal diners to enter
        sem_post(&entering_diners);

        // Wait for all diners to enter
        for (int i = 0; i < N; i++) {
            sem_wait(&enter_done);
        }

        // Close front door
        closeFrontDoor();

        // Serve food
        serveFood();

        // Open back door
        openBackDoor();

        // Signal diners to exit
        sem_post(&exiting_diners);

        // Wait for all diners to exit
        for (int i = 0; i < N; i++) {
            sem_wait(&exit_done);
        }

        // Close back door
        closeBackDoor();
    }

    return NULL;
}

void *diner_thread(void *arg) {
    while (1) {
        // Wait for signal to enter
        sem_wait(&entering_diners);

        // Enter the restaurant
        enterRestaurant();

        // Increment enter counter with mutual exclusion
        sem_wait(&mutex_enter);
        count_enter++;
        if (count_enter < N) {
            sem_post(&entering_diners);
        } else if (count_enter == N) {
            sem_post(&enter_done);
            count_enter = 0;
        }
        sem_post(&mutex_enter);

        // Eat
        eat();

        // Wait for signal to exit
        sem_wait(&exiting_diners);

        // Exit the restaurant
        exitRestaurant();

        // Increment exit counter with mutual exclusion
        sem_wait(&mutex_exit);
        count_exit++;
        if (count_exit < N) {
            sem_post(&exiting_diners);
        } else if (count_exit == N) {
            sem_post(&exit_done);
            count_exit = 0;
        }
        sem_post(&mutex_exit);
    }

    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&entering_diners, 0, 0);
    sem_init(&exiting_diners, 0, 0);
    sem_init(&enter_done, 0, 0);
    sem_init(&exit_done, 0, 0);
    sem_init(&mutex_enter, 0, 1);
    sem_init(&mutex_exit, 0, 1);

    // Create threads
    pthread_t restaurant, diner;
    pthread_create(&restaurant, NULL, restaurant_thread, NULL);
    pthread_create(&diner, NULL, diner_thread, NULL);

    // Wait for threads to finish
    pthread_join(restaurant, NULL);
    pthread_join(diner, NULL);

    // Destroy semaphores
    sem_destroy(&entering_diners);
    sem_destroy(&exiting_diners);
    sem_destroy(&enter_done);
    sem_destroy(&exit_done);
    sem_destroy(&mutex_enter);
    sem_destroy(&mutex_exit);

    return 0;
}
