
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PRODUCED 17
#define MAX_QUEUE 7

#define NUM_PRODUCERS 200
#define NUM_CONSUMERS 200

pthread_cond_t cond_queue_empty, cond_queue_full;
pthread_mutex_t item_queue_cond_lock, produced_lock, consumer_lock, item_lock;

int queue[MAX_QUEUE], item_available=0, produced=0, consumed=0;

int create_item(void) {
	return(rand()%1000);
}

void insert_into_queue(int tid, int item) {
	queue[item_available++] = item;
    produced++;
	printf("[%d] producing item:%d, value:%d, queued:%d \n", tid, produced, item, item_available);
    usleep(100*1000);
	return;

}

int extract_from_queue(int tid) {
	consumed++;
	printf("[%d] consuming item:%d, value:%d, queued:%d \n", tid, consumed, queue[item_available-1], item_available-1); 
	int extracted = (queue[--item_available]);
    return extracted;
}

void process_item(int tid, int my_item) {
	static int printed=0;

	printf("[%d] Printed:%d, value:%d, queued:%d \n", tid, printed++, my_item, item_available);

	return;

}

void *producer(void *tid) {
    int thread_id = *((int *)tid);
    // printf("[%d] Producer starting...", thread_id);

	int item;
	while (1) {
		item = create_item();

		pthread_mutex_lock(&item_queue_cond_lock);

        // broadcast to wake other producers
        if (produced >= MAX_PRODUCED) {
            pthread_cond_broadcast(&cond_queue_empty);
            pthread_mutex_unlock(&item_queue_cond_lock);
            break;
        }

        while (item_available == MAX_QUEUE) {
            printf("[%d] Queue full waiting consumers... (produced: %d)\n", thread_id, produced);
			pthread_cond_wait(&cond_queue_empty, &item_queue_cond_lock);
        }

        if (produced >= MAX_PRODUCED) {
            pthread_mutex_unlock(&item_queue_cond_lock);
            break;
        }

		insert_into_queue(*((int *)tid), item);

        pthread_cond_signal(&cond_queue_full);
        pthread_mutex_unlock(&item_queue_cond_lock);
	}

    printf("[%d] Producer exit...\n", thread_id);
	pthread_exit(0);
}

void *consumer(void *tid) {
    int thread_id = *((int *)tid);
    // printf("[%d] Consumer starting...", thread_id);

	int my_item = 0;
	while (consumed < MAX_PRODUCED) {
		pthread_mutex_lock(&item_queue_cond_lock);

        if (consumed >= MAX_PRODUCED) {
            pthread_mutex_unlock(&item_queue_cond_lock);
            break;
        }

        while (item_available == 0) {
            if (consumed >= MAX_PRODUCED) {  // prevent deadlock
                pthread_mutex_unlock(&item_queue_cond_lock);
                pthread_exit(0);
            }

            printf("[%d] No item available. Waiting producers...\n", thread_id);
            pthread_cond_wait(&cond_queue_full, &item_queue_cond_lock);
        }

        my_item = extract_from_queue(thread_id);

        pthread_cond_signal(&cond_queue_empty);
        pthread_mutex_unlock(&item_queue_cond_lock);
        process_item(thread_id, my_item);
	}

    printf("[%d] Consumer exit...\n", thread_id);
	pthread_exit(0);
}

int main(void) {
	pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    item_available = 0;

    pthread_cond_init(&cond_queue_empty, NULL);
    pthread_cond_init(&cond_queue_full, NULL);
    pthread_mutex_init(&item_queue_cond_lock, NULL);

    for(int i = 0; i < NUM_PRODUCERS; i++) {
        int *tid = malloc(sizeof(int));
        *tid = i;

        if (pthread_create(&producers[i], 0, (void *) producer, (void *) tid)) { 
            printf("Error creating thread producer! Exiting! \n");
            exit(0);
        }
    }

    for(int i = 0; i < NUM_CONSUMERS; i++) {
        int *tid = malloc(sizeof(int));
        *tid = i;

        if (pthread_create(&consumers[i], 0, (void *) consumer, (void *) tid)) { 
            printf("Error creating thread consumer! Exiting! \n");
            exit(0);
        }
    }

    for(int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], 0);
    }

    for(int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], 0);
    }

    printf("Exiting main thread. Produced %d, consumed %d, available %d", produced, consumed, item_available);

	exit(0);	
}
