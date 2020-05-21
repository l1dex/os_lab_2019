#include <getopt.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int A;
int B;

void AB();
void BA();

pthread_mutex_t mutA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutB = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t thread1;
    pthread_t thread2;

    if (pthread_create(&thread1, NULL, (void *)AB,
                    NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, (void *)BA,
                    NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    printf("ds.kdsjlkhfdslsejesliu\n");

}

void AB()
{
    pthread_mutex_lock(&mutA);
    printf("AB loc A\n");
    pthread_mutex_unlock(&mutA);
    sleep(1);
        pthread_mutex_lock(&mutB);
    
        pthread_mutex_unlock(&mutB);
    //pthread_mutex_unlock(&mutA);
}

void BA()
{
       pthread_mutex_lock(&mutB);
           printf("BA loc B\n");
        pthread_mutex_unlock(&mutB);
   sleep(1);
        pthread_mutex_lock(&mutA);
    
        pthread_mutex_unlock(&mutA);
    //pthread_mutex_unlock(&mutB);
}