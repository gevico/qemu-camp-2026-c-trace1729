#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head; // points to next enqueue position
    int tail; // points to the first element
    int count; // count the number of alive people, do not use ptr to determine the size, since we need MAX_PEOPLE + 1
    // to distinguish full and empty.
} Queue;

void enqueue (Queue* q, People x) {
    q->data[q->tail] = x;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
}

People dequeue (Queue* q) {
    People p = q->data[q->head];
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return p;
}

size_t len(Queue* q) {
    if (q->tail > q->head) {return q->tail - q->head ;}
    else {return q->tail - q->head + MAX_PEOPLE;}
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;
    q.head = 0, q.tail = 0, q.count = 0;

    for (int i = 0; i < MAX_PEOPLE; i++) {
        People p; p.id = i + 1;
        enqueue(&q, p);
    }

    while (q.count > 1) {
        // rotate the frist four people to the tail (means moves to the next round)
        for (int i = 0; i < 4; i++) {
            People p = dequeue(&q);
            enqueue(&q, p);
        }
        // discard the 5th people
        People p = dequeue(&q);
        printf("淘汰: %d\n", p.id);
    }

    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}
