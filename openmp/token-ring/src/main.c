
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 4

int main(void) {
    int token = 0;
    int task = 0;

    #pragma omp parallel num_threads(NUM_THREADS) shared(token)
    {
        #pragma omp single
        {
            #pragma omp task depend(out: token)
            {
                int tid = omp_get_thread_num();
                printf("[%d] executed by thread %d, token = %d\n", task++, tid, token);
                token = 0; // only passes the first token to t1
            }

            #pragma omp task depend(inout: token)
            {
                int tid = omp_get_thread_num();

                printf("[%d] executed by thread %d, token = %d\n", task, tid, token);

                #pragma omp task depend(inout: token)
                {
                    int tid2 = omp_get_thread_num();
                    printf("[%d][0] executed by thread %d, token = %d\n", task, tid2, token++);
                }

                #pragma omp task depend(inout: token)
                {
                    int tid3 = omp_get_thread_num();
                    printf("[%d][1] executed by thread %d, token = %d\n", task, tid3, token++);
                }

                #pragma omp task depend(inout: token)
                {
                    int tid4 = omp_get_thread_num();
                    printf("[%d][2] executed by thread %d, token = %d\n", task, tid4, token++);
                }
                #pragma omp taskwait

                printf("[%d] executed by thread %d, token = %d\n", task++, tid, token++);
            }

            #pragma omp task depend(inout: token)
            {
                int tid = omp_get_thread_num();
                printf("[%d] executed by thread %d, token = %d\n", task, tid, token);

                #pragma omp task depend(inout: token)
                {
                    int tid2 = omp_get_thread_num();
                    printf("[%d][0] executed by thread %d, token = %d\n", task, tid2, token++);
                }

                #pragma omp task depend(inout: token)
                {
                    int tid3 = omp_get_thread_num();
                    printf("[%d][1] executed by thread %d, token = %d\n", task, tid3, token++);
                }

                #pragma omp task depend(inout: token)
                {
                    int tid4 = omp_get_thread_num();
                    printf("[%d][2] executed by thread %d, token = %d\n", task, tid4, token++);
                }
                #pragma omp taskwait

                printf("[%d] executed by thread %d, token = %d\n", task++, tid, token++);
            }

            #pragma omp task depend(inout: token)
            {
                int tid = omp_get_thread_num();
                printf("[%d] executed by thread %d, token = %d\n", task, tid, token);

                #pragma omp task depend(inout: token)
                {
                    int tid2 = omp_get_thread_num();
                    printf("[%d][0] executed by thread %d, token = %d\n", task, tid2, token++);
                }

                #pragma omp task depend(inout: token)
                {
                    int tid3 = omp_get_thread_num();
                    printf("[%d][1] executed by thread %d, token = %d\n", task, tid3, token++);
                }

                #pragma omp task depend(inout: token)
                {
                    int tid4 = omp_get_thread_num();
                    printf("[%d][2] executed by thread %d, token = %d\n", task, tid4, token++);
                }
                #pragma omp taskwait

                printf("[%d] executed by thread %d, token = %d\n", task++, tid, token++);
            }

            #pragma omp taskwait

            task = 0;
            int tid = omp_get_thread_num();
            printf("[%d] executed by thread %d, token = %d\n", task, tid, token);

            #pragma omp task depend(inout: token)
            {
                int tid2 = omp_get_thread_num();
                printf("[%d][0] executed by thread %d, token = %d\n", task, tid2, token++);
            }

            #pragma omp task depend(inout: token)
            {
                int tid3 = omp_get_thread_num();
                printf("[%d][1] executed by thread %d, token = %d\n", task, tid3, token++);
            }

            #pragma omp task depend(inout: token)
            {
                int tid4 = omp_get_thread_num();
                printf("[%d][2] executed by thread %d, token = %d\n", task, tid4, token++);
            }
            #pragma omp taskwait

            printf("[%d] executed by thread %d, token = %d\n", task++, tid, token++);
        }

    }

    printf("Final token value = %d\n", token);

    return 0;
}
