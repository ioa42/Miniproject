#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hash_table.h"

uint32_t hash(char *data) {
    uint32_t h = 0;

    for(int i = 0; data[i]; ++i) {
        h += (int)data[i];
        h += h << 10;
        h = h ^ (h >> 6);
    }
    h += h << 3;
    h = h ^ (h >> 11);
    h += h << 15;
    return h;
}

struct htable *create_htable(size_t capacity) {
   struct htable *table = malloc(sizeof(struct htable));
   struct pair **array   = malloc(capacity * sizeof(struct pair*));

    for(size_t i = 0; i < capacity; i++) {
        *(array + i) = NULL;
    }

   table->size          = 0;
   table->capacity      = capacity;
   table->tab           = array;
   return table;
}


void clearTab(struct pair **tab, size_t size) {
    for(size_t i = 0; i < size; ++i) {
        struct pair *current = tab[i];
        struct pair *nxt     = NULL;
        for(; current; current = nxt) {
            nxt = current->next;
            free(current->key);
            free(current->value);
            free(current);
        }
    }

}

void clear_htable(struct htable *h) {
    clearTab(h->tab, h->capacity);

    free(h->tab);
    free(h);
}

void disp(struct htable *h) {
    for(size_t i = 0; i < h->capacity; ++i) { 
        struct pair *current = h->tab[i];
        printf("\n[%2lu] : ", i);
        for(; current; current = current->next) {
            printf("{key: %s, hashKey: %10u} ->", current->key, current->hkey);
        }
    }
    printf("\n");


}

int addTable(struct pair **tab, char *k, uint32_t hashKey, void *v, int i) {

    struct pair *p = *(tab + i);
    struct pair *prev = NULL;
    for(; p; p = p->next) {
        if(p->key == k) { return 0; }
        prev = p;
    }

    p = malloc(sizeof(struct pair));
    p->hkey = hashKey;
    p->key = k;
    p->value = v;
    p->next = NULL;
    if(prev == NULL) { tab[i] = p; }
    else             { prev->next = p;   }

    return 1;
}

int add_htable(struct htable *h, char *k, void *v) {

    if(h->size == 0 || (float)h->size / h->capacity > .75) {
        struct pair **array   = malloc(h->capacity * 2 * sizeof(struct pair*));

        for(size_t i = 0; i < h->capacity * 2; i++) {
            array[i] = NULL;
        }

        for(size_t i = 0; i < h->capacity; i++) {
            struct pair *curr = h->tab[i];
            struct pair *prev = NULL;
            for(; curr;) {
                prev = curr;
                addTable(array, curr->key, curr->hkey,
                         curr->value, curr->hkey % (h->capacity * 2));
                curr = curr->next;
                free(prev);
            }
        }

        free(h->tab);
        h->tab = array;
        h->capacity *= 2;
    }
    
    uint32_t hashKey = hash(k);
    uint32_t i = hashKey %  h->capacity;

    if(!addTable(h->tab, k, hashKey, v, i)) { return 0; }
    h->size += 1;
    return 1;

}

void remove_htable(struct htable *h, char *k) {
    uint32_t hashKey = hash(k);
    uint32_t i = hashKey %  h->capacity;

    struct pair *current = h->tab[i];
    struct pair *prev    = NULL;
    for(; current; current = current->next) {
        if(strcmp(current->key, k) == 0) {
            if(prev == NULL) { h->tab[i] = current->next; }
            else { prev->next = current->next; }
            free(current->key);
            free(current->value);
            free(current);
        }
        prev = current;
    }

}

struct pair *access_htable(struct htable *h, char *k) {
    uint32_t hashKey = hash(k);
    uint32_t i = hashKey %  h->capacity;

    struct pair *current = h->tab[i];
    for(; current; current = current->next) {
        if(strcmp(current->key, k) == 0) { return current; }
    }
    return NULL;
}
