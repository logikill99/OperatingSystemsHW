#include <iostream>
#include <pthread.h>
#include <semaphore.h>

// Semaphore declaration
sem_t sem;

// Function that the first thread will run
void *runMeFirst(void *arg) {
    std::cout << "Run me first" << std::endl;
    // Signal the second thread that it can proceed
    sem_post(&sem);
    return nullptr;
}

// Function that the second thread will run
void *runMeSecond(void *arg) {
    // Wait for the signal from the first thread
    sem_wait(&sem);
    std::cout << "I run second" << std::endl;
    return nullptr;
}

void *runMeThird(void *arg) {
    sem_wait(&sem);
    std::cout << "I run third" << std::endl;
    return nullptr;
}

int main() {
    // Initialize semaphore
    sem_init(&sem, 0, 0);

    pthread_t t1, t2, t3;

    // Create threads
    pthread_create(&t1, nullptr, runMeFirst, nullptr);
    pthread_create(&t2, nullptr, runMeSecond, nullptr);
    pthread_create(&t3, nullptr, runMeThird, nullptr);

    // Wait for threads to finish
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);

    // Cleanup semaphore
    sem_destroy(&sem);

    return 0;
}
