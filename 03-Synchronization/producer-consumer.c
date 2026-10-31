#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define ITEMS_TO_PRODUCE 10
#define STOP_VALUE -1

#define EMPTY_NAME "/os_cap_empty_20261031"
#define FULL_NAME  "/os_cap_full_20261031"
#define MUTEX_NAME "/os_cap_mutex_20261031"

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t *empty_slots;
sem_t *full_slots;
sem_t *mutex;

void wait_sem(sem_t *s) {
    if (sem_wait(s) == -1) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

void post_sem(sem_t *s) {
    if (sem_post(s) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}

void *producer(void *arg) {
    (void)arg;

    for (int i = 1; i <= ITEMS_TO_PRODUCE; i++) {
        wait_sem(empty_slots);
        wait_sem(mutex);

        buffer[in] = i;
        printf("Produced: %d\n", i);
        in = (in + 1) % BUFFER_SIZE;

        post_sem(mutex);
        post_sem(full_slots);
    }

    wait_sem(empty_slots);
    wait_sem(mutex);

    buffer[in] = STOP_VALUE;
    in = (in + 1) % BUFFER_SIZE;

    post_sem(mutex);
    post_sem(full_slots);

    return NULL;
}

void *consumer(void *arg) {
    (void)arg;

    while (1) {
        wait_sem(full_slots);
        wait_sem(mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        post_sem(mutex);
        post_sem(empty_slots);

        if (item == STOP_VALUE)
            break;

        printf("Consumed: %d\n", item);
    }

    return NULL;
}

int main(void) {
    pthread_t p, c;

    sem_unlink(EMPTY_NAME);
    sem_unlink(FULL_NAME);
    sem_unlink(MUTEX_NAME);

    empty_slots = sem_open(EMPTY_NAME, O_CREAT, 0600, BUFFER_SIZE);
    full_slots = sem_open(FULL_NAME, O_CREAT, 0600, 0);
    mutex = sem_open(MUTEX_NAME, O_CREAT, 0600, 1);

    if (empty_slots == SEM_FAILED ||
        full_slots == SEM_FAILED ||
        mutex == SEM_FAILED) {
        perror("sem_open");
        return EXIT_FAILURE;
    }

    if (pthread_create(&p, NULL, producer, NULL) != 0 ||
        pthread_create(&c, NULL, consumer, NULL) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_close(empty_slots);
    sem_close(full_slots);
    sem_close(mutex);

    sem_unlink(EMPTY_NAME);
    sem_unlink(FULL_NAME);
    sem_unlink(MUTEX_NAME);

    printf("\nProgram completed successfully!\n");

    return 0;
}