#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// q->front
// q->rear 指向下一个出队的元素

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode* new_node = calloc(1, sizeof(QueueNode));
    new_node->tree_node = tree_node;

    if (q->front == NULL && q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->front->next = new_node;
        q->front = new_node;
    }


}

TreeNode* dequeue(Queue *q) {
    if (!q->rear) {
        return NULL;
    }
    QueueNode* t = q->rear;
    TreeNode* val = t->tree_node;
    q->rear = q->rear->next;
    if (q->rear == NULL) {q->front = NULL;}
    free(t);
    return val;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    TreeNode* root = calloc(1, sizeof(TreeNode));
    root->val = level_order[0];

    // initial state, transition conditon and termination condition
    // general for loop contains only one state, if you have multiple pending state, you need queue.
    Queue* pendig = create_queue();
    enqueue(pendig, root);
    // 层次遍历，idx 是自然递增的
    int idx = 1;
    while (!is_empty(pendig)) {
        TreeNode* t = dequeue(pendig);
        if (!t) {
            printf("error!");
            exit(-1);
        }
        if (idx < size && level_order[idx] != INT_MIN) {
            TreeNode* left = calloc(1, sizeof(TreeNode));
            left->val = level_order[idx];
            t->left = left;
            enqueue(pendig, left);
        }
        idx++;
        if (idx < size && level_order[idx] != INT_MIN) {
            TreeNode* right = calloc(1, sizeof(TreeNode));
            right->val = level_order[idx];
            t->right = right;
            enqueue(pendig, right);
        }
        idx++;
    }
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (!root) {return ; }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    QueueNode *stack = calloc(1, sizeof(QueueNode));
    stack->tree_node = root;

    while (stack != NULL) {
        QueueNode *top = stack;
        TreeNode *t = top->tree_node;
        stack = stack->next;
        free(top);

        printf("%d ", t->val);

        if (t->right != NULL) {
            QueueNode *node = calloc(1, sizeof(QueueNode));
            node->tree_node = t->right;
            node->next = stack;
            stack = node;
        }
        if (t->left != NULL) {
            QueueNode *node = calloc(1, sizeof(QueueNode));
            node->tree_node = t->left;
            node->next = stack;
            stack = node;
        }
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
