#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    Node* head = calloc(1, sizeof(Node));
    if (!head) {printf("error.\n"); return NULL;}
    head->id = 1;

    Node* prev = head;
    for (int i = 2; i <= n; i++) {
        Node* cur = calloc(1, sizeof(Node));
        cur->id = i;
        if (!cur) {printf("error.\n"); return NULL;}
        prev->next = cur;
        prev = cur;
    }
    if (prev->id != n) {
        printf("WA\n");
        exit(-1);
    };
    prev->next = head;
    return head;
}

void free_list(Node* head) {
    while (head != NULL) {
        Node* cur = head;
        head = head->next;
        free(cur);
    }
}
