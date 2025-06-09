#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10
int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0; // Buffer control variables

int produce_odd() {
    return (rand() % 9765) * 2 + 31; // creates randomly number
}

void* producer(void* arg) {
    while (1) {
        if (count < BUFFER_SIZE) { // control whether empty space in buffer or not
            int item = produce_odd();
            buffer[in] = item; // add created data into buffer
            printf("Produced: %d at index %d\n", item, in);
            in = (in + 1) % BUFFER_SIZE; // increase index for circular buffer
            count++;
        } else {
            printf("Buffer is full, producer is waiting...\n");
        }
        sleep(1); // wait after production
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        if (count > 0) { // control there is data in buffer or not
            int item = buffer[out]; // take data from buffer
            printf("Consumed: %d from index %d\n", item, out);
            out = (out + 1) % BUFFER_SIZE; // increase index for circular buffer
            count--;
        } else {
            printf("Buffer is empty, consumer is waiting...\n");
        }
        sleep(1); // wait after consumption
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    //keep waiting threads (program always Works because of infinite loop)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
