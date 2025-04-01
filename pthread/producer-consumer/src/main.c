#define NUM_ELEM_TO_PRODUCE 17
#define BUFFER_SIZE 7

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t empty_count;
sem_t fill_count;
pthread_mutex_t mutex;

struct BufferState {
    int buffer[BUFFER_SIZE];
    int buffer_index;
};

void *produce(void *bufferState) {
    printf("Producer thread started.\n");
    struct BufferState *bs = (struct BufferState *)bufferState;

    for (int i = 0; i < NUM_ELEM_TO_PRODUCE; i++) {
        int value = rand() % 100;

        sem_wait(&empty_count);
        pthread_mutex_lock(&mutex);

        bs->buffer[bs->buffer_index++] = value;
        printf("Produced: %d\n", value);

        pthread_mutex_unlock(&mutex);
        sem_post(&fill_count);
    }

    return NULL;
}

void *consume(void *bufferState) {
    printf("Consumer thread started.\n");
    struct BufferState *bs = (struct BufferState *)bufferState;

    while (1) {
        sem_wait(&fill_count);
        pthread_mutex_lock(&mutex);

        if (bs->buffer_index <= 0 && NUM_ELEM_TO_PRODUCE <= 0) {
            pthread_mutex_unlock(&mutex);
            sem_post(&fill_count);
            break;
        }

        int value = bs->buffer[--bs->buffer_index];
        printf("Consumed: %d\n", value);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_count);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t producer, consumer;
    struct BufferState bufferState;
    bufferState.buffer_index = 0;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_count, 0, BUFFER_SIZE);
    sem_init(&fill_count, 0, 0);

    if (pthread_create(&producer, NULL, produce, (void *)&bufferState) != 0) {
        fprintf(stderr, "Error creating producer thread\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&consumer, NULL, consume, (void *)&bufferState) != 0) {
        fprintf(stderr, "Error creating consumer thread\n");
        exit(EXIT_FAILURE);
    }


    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_count);
    sem_destroy(&fill_count);

    printf("Main thread exiting.\n");
    return 0;
}
