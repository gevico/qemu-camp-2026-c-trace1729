#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

#define DEBUG

#define ASSERT(cond) \
do { \
    if (((cond) == 0))  { \
        printf("assert func=%s, line=%d\n", __func__, __LINE__); \
        exit(1);\
    } \
} while(0);

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(LRUCache* c, int key) {
    return key % (c->bucket_count);
}

/*
 *  while (*link && (*link)->key != key) {
 *     smarter way.
 *      // link stores the handle of all the next ptr of the bucket list
 *      link = &(*link)->next;
    }
    // when the loop end, link stores ptr to the "next" field that statisfy the next->key == key.
 * */


/*
 *  The pointer are just wrapper around of how memory access a pointer can resolve to.
 * */
static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    DEBUG
    ASSERT(hash_int(c, key) < c->bucket_count);
    HashEntry* head = c->buckets[hash_int(c, key)];
    HashEntry* prev = head;
    ASSERT(head != NULL);
    head = head->next; // head is sential

    while (head != NULL && head->node != NULL && head->node->key != key) {
        prev = head;
        head = head->next;
    }
    // if pprev_next is not NULL, the hashmap must contain the target.
    if (pprev_next != NULL) {
        ASSERT(prev != NULL);
        *pprev_next = &(prev->next);
    }

    return head;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    DEBUG
    ASSERT(node != NULL);
    node->next = c->head->next;
    c->head->next = node;
    node->next->prev = node;
    node->prev = c->head;
}

/* static void list_remove(LRUCache* c, LRUNode* node) { */
/*     DEBUG */
/*     ASSERT(node != NULL); */
/*     LRUNode* p = c->head; */
/*     while (p != c->tail && p != node) {p = p->next;} */
/*     if (p == c->tail) {return;} */
/*     ASSERT(node->prev); */
/*     ASSERT(node->next); */
/*     node->prev->next = node->next; */
/*     node->next->prev = node->prev; */
/*     free(node); */
/* } */

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    DEBUG
    ASSERT(node != NULL);
    LRUNode* p = c->head;
    while (p != c->tail && p != node) {p = p->next;}
    if (p == c->tail) {return;}
    node->prev->next = node->next;
    node->next->prev = node->prev;
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    DEBUG
    ASSERT(c->tail != NULL);
    if (c->tail->prev == c->head) {return NULL;}
    LRUNode* evict = c->tail->prev;
    ASSERT(c->tail->prev != NULL);
    ASSERT(c->tail->prev->prev != NULL);
    c->tail->prev->prev->next = c->tail;
    c->tail->prev = c->tail->prev->prev;
    return evict;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    DEBUG
    // 0. inint lrucache
    LRUCache* lru = calloc(1, sizeof(LRUCache));
    lru->capacity = capacity;
    // 1. init list
    lru->head = calloc(1, sizeof(LRUNode));
    lru->tail = calloc(1, sizeof(LRUNode));
    lru->head->next = lru->tail;
    lru->tail->prev = lru->head;
    // 2. init hashmap
    lru->buckets = calloc(capacity, sizeof(HashEntry*));
    for (int i = 0; i < capacity; i++) {
        lru->buckets[i] = calloc(1, sizeof(HashEntry));
    }
    lru->bucket_count = 3;
    return lru;
}

static void lru_free(LRUCache* c) {
    DEBUG
    // free_list
    ASSERT(c->head != NULL && c->tail != NULL);
    LRUNode* p = c->head;
    while(p != c->tail) {
        LRUNode* t = p;
        p = p->next;
        free(t);
    }
    free(c->tail);

    // free hash_map
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry* head = c->buckets[i];
        while (head != NULL) {
            HashEntry* t = head;
            head = head->next;
            free(t);
        }
    }
    free(c->buckets);
    // free lru
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    DEBUG
    HashEntry* n = hash_find(c, key, NULL);
    if (n == NULL) {
        return -1;
    }
    ASSERT(n->node != NULL);
    *out_value = n->node->value;
    list_move_to_head(c, n->node);
    return 0;
}

static void hash_insert(LRUCache* c, int key, LRUNode* data) {
    DEBUG
    ASSERT(data != NULL);
    HashEntry* entry = calloc(1, sizeof(HashEntry));
    entry->key = key;
    entry->node = data;
    int idx = hash_int(c, entry->key);
    HashEntry* bucket_head = c->buckets[idx];
    entry->next = bucket_head->next;
    bucket_head->next = entry;

}

static void lru_evict(LRUCache* c) {
    DEBUG
    // link_list remove
    LRUNode* evict = list_pop_tail(c);
    ASSERT(evict != NULL);
    // hash_remove
    HashEntry** pprev_next = NULL; // "generlize" the op which find the ptr that points to the node before the target node;
    HashEntry* n = hash_find(c, evict->key, &pprev_next);
    ASSERT(pprev_next != NULL);
    *pprev_next = n->next;
    free(n);
    free(evict);
    c->size--;
}

static void lru_put(LRUCache* c, int key, int value) {
    DEBUG

    HashEntry* n = hash_find(c, key, NULL);
    if (n != NULL) {
        n->node->value = value;
        list_move_to_head(c, n->node);
    } else {
        if (c->size + 1 > c->capacity) {
            lru_evict(c);
        }
        // link_list insert
        LRUNode* data = calloc(1, sizeof(LRUNode));
        data->key = key, data->value = value;
        list_add_to_head(c, data);
        // hash_map insert
        hash_insert(c, key, data);
        c->size++;
    }
}


/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* lru) {
    DEBUG
    ASSERT(lru->head != NULL && lru->tail != NULL);
    LRUNode* p = lru->head->next;
    int flag = 1;
    while(p != lru->tail) {
        if (!flag) printf(", ");
        flag = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    DEBUG
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
