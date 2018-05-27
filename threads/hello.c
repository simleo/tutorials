#include <pthread.h>
#include <stdio.h>


void *output(void*);


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int main(void) {
    pthread_t t1, t2;
    const char* head = "Hello, ";
    const char* tail = "World. ";

    pthread_create(&t1, NULL, output, (void*)head);
    pthread_create(&t2, NULL, output, (void*)tail);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("\n");

    return 0;
}


void *output(void *msg) {
    int i;
    for(i = 0; i < 10; ++i) {
	pthread_mutex_lock(&lock);
	printf("%s\n", (char*)msg);
	pthread_mutex_unlock(&lock);
    }
    return NULL;
}
