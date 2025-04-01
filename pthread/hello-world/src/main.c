#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Include for strerror
#include <pthread.h>

void *print_message(void *arg)
{
    char* message = (char*) arg;
    printf("Thread says: %s\n", message);

    char* retval = malloc(20);
    strcpy(retval, "Finished!");
    return retval;
}

int main() {
    pthread_t thread1, thread2;
    int ret1, ret2;
    void *res1, *res2;

    char *msg1 = "Hello from Thread 1";
    char *msg2 = "Hello from Thread 2";

    ret1 = pthread_create(&thread1, NULL, print_message, (void*)msg1);
    if (ret1) {
        fprintf(stderr, "Error creating thread 1: %s\n", strerror(ret1));
        exit(EXIT_FAILURE);
    }

    ret2 = pthread_create(&thread2, NULL, print_message, (void*)msg2);
    if (ret2) {
        fprintf(stderr, "Error creating thread 2: %s\n", strerror(ret2));
        exit(EXIT_FAILURE);
    }

    // Join threads and collect their return values
    pthread_join(thread1, &res1);
    pthread_join(thread2, &res2);

    printf("Thread 1 returned: %s\n", (char*)res1);
    printf("Thread 2 returned: %s\n", (char*)res2);

    free(res1);
    free(res2);

    printf("Main thread exiting.\n");
    return 0;
}
