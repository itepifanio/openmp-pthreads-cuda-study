
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define INITIAL_TOKEN { .value = 0 }

#define NUM_COMPUTERS 3

sem_t main_sem;
sem_t semaphores[NUM_COMPUTERS];

struct Token {
    int value;
};

struct Token token = INITIAL_TOKEN;

void communicate(void *tid) {
    int threadId = *((int *) tid);
    int next = (threadId+1) % NUM_COMPUTERS;

    sem_wait(&semaphores[threadId]);

    printf(
        "📬 t%d Received %d\n", 
        threadId,
        token.value
    );
    // sleep(1);
    token.value += 10;

    if (threadId < NUM_COMPUTERS-1) {
        printf("📨 t%d Sending %d to t%d\n", threadId, token.value, next);
        sem_post(&semaphores[next]);
    } else {
        printf("📨 t%d Sending %d to main\n", threadId, token.value);
        sem_post(&main_sem);
    }
}

int main(void) {
	pthread_t computers[NUM_COMPUTERS];

    for(int i = 0; i < NUM_COMPUTERS; i++) {
        sem_init(&semaphores[i], 0, 0);
    }
    sem_init(&main_sem, 0, 0);

    for(int i = 0; i < NUM_COMPUTERS; i++) {
        int *index = malloc(sizeof(int));
        *index = i;

        if (pthread_create(&computers[i], 0, (void *) communicate, (void *) index)) { 
            printf("Error creating thread producer! Exiting! \n");
            exit(0);
        }
    }

    printf("📨 main sending %d to t%d\n", 0, token.value, 1);
    sem_post(&semaphores[0]);

    sem_wait(&main_sem);

    for(int i = 0; i < NUM_COMPUTERS; i++) {
        pthread_join(computers[i], NULL);
    }

    for(int i = 0; i < NUM_COMPUTERS; i++) {
        sem_destroy(&semaphores[i]);
    }
    sem_destroy(&main_sem);

    printf("📬 main received %d\n", token.value);

	exit(0);	
}
