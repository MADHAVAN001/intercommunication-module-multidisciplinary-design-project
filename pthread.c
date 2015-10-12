#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_var1 = 0; // global variable
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *inc_gv()
{       int i,j;

        for (i=0;i<10;i++)
                { pthread_mutex_lock(&mutex1);
                  g_var1++; // increment the global variable
                  for (j=0; j<5000000;j++); // delay loop
                  printf(" %d",g_var1); // print the value
                  fflush(stdout);
                  pthread_mutex_unlock(&mutex1);
                }

}

main() {
        pthread_t TA, TB;
        int TAret, TBret;

        /*TAret = pthread_create(&TA, NULL, inc_gv(), NULL);
        TBret = pthread_create(&TB, NULL, inc_gv(), NULL);*/

        if((TAret = pthread_create(&TA, NULL, &inc_gv, NULL))) {
                printf("Thread creation failed: %d\n", TAret);
        }

        if((TBret = pthread_create(&TB, NULL, &inc_gv, NULL))) {
                printf("Thread creation failed: %d\n", TBret);
        }

        pthread_join(TA, NULL);
        pthread_join(TB, NULL);
        pthread_mutex_destroy(&mutex1);

        printf("\nTA & TB return: %d, %d\n", TAret, TBret);
        printf("pthread2 completed\n");
}
