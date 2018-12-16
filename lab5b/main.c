#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>

pthread_mutex_t mutex;

void* producer(void *args) {
        char * addr;
        int shmid;
        shmid = (shmget(2002, 32, IPC_CREAT | 0666));

        if( shmid == -1 ){
                printf("Can't create shared memory\n");
                exit(0);
        }

        if((addr = shmat( shmid, NULL, 0 )) == (char*)-1){
                printf("Shmat err\n");
                exit(0);
        }

        time_t timer = time(0);
        //time_t buft = timer;
        while(1){
                timer = time(0);
                pthread_mutex_lock(&mutex);
                        sprintf(addr, "%s", ctime(&timer));
                pthread_mutex_unlock(&mutex);
        }
        return 0;
}

int main() {
        pthread_t thread;
        int status;

        pthread_mutex_init(&mutex, NULL);

        status = pthread_create(&thread, NULL, producer, NULL);
        if (status != 0) {
                printf("main error: can't create thread, status = %d\n", status);
                exit(0);
        }
        sleep(1);

        char * addr;
        int shmid;
        shmid = (shmget(2002, 32, 0666));
        if( shmid == -1 ){
                printf("Can't open shared memory\n");
                exit(0);
        }
        if((addr = shmat( shmid, NULL, 0 )) == (char*)-1){
                printf("Shmat err\n");
                exit(0);
        }
        while(1){
                pthread_mutex_lock(&mutex);
                        printf("%s", addr);
                pthread_mutex_unlock(&mutex);
                sleep(1);
        }

        pthread_mutex_destroy(&mutex);
        return 0;
}