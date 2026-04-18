#include "doubly_circular_queue.h"

#include <stdlib.h>
#include <stdio.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

// guardian, headsentinel-> next points to the first enqueue element
// tailsentinel-> prev points to the first dequeue element

typedef struct node Node;

link make_node(int data) {
    link new_node = calloc(1, sizeof(Node));
    new_node->data = data;
    return new_node;
}

void free_node(link p) {
    free(p);
}

link search(int key) {
    link cur = headsentinel.next;
    while (cur != &tailsentinel) {
        if (cur->data == key) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void insert(link p) {
    link old_first = headsentinel.next;
    headsentinel.next = p;
    p->next = old_first;
    old_first->prev = p;
    p->prev = head;
}

void delete(link p) {
    link to_del = search(p->data);
    link prev = to_del->prev;
    link next = to_del->next;
    prev->next = next;
    next->prev = prev;
    free(to_del);
}

void traverse(void (*visit)(link)) {
    link cur = headsentinel.next;
    while (cur != &tailsentinel) {
        visit(cur);
        cur = cur->next;
    }
}

void destroy(void) {
    link cur = headsentinel.next;
    while (cur != &tailsentinel) {
        free(cur);
        cur = cur->next;
    }
}
