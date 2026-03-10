#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct TableEntry {
    unsigned item_id;
    unsigned price;
    struct TableEntry *next;
} TableEntry;

typedef struct {
    TableEntry **entries;
    size_t cap, sz;
} HashTable;

TableEntry *new_entry(unsigned item_id, unsigned price) {
    TableEntry *entry = malloc(sizeof(TableEntry));
    assert(entry);
    entry->item_id = item_id;
    entry->price = price;
    entry->next = NULL;
    return entry;
}

HashTable *new_hash_table(size_t cap) {
    assert(cap > 0);

    HashTable *ht = malloc(sizeof(HashTable));
    assert(ht);
    ht->cap = cap;
    ht->sz = 0;

    // All NULL pointers
    ht->entries = calloc(ht->cap, sizeof(*ht->entries));
    assert(ht->entries);
    return ht;
}

void free_hash_table(HashTable *ht) {
    if (ht) {
        if (ht->entries) {
            for (size_t i = 0; i < ht->cap; i++) {
                TableEntry *e = ht->entries[i];
                while (e) {
                    TableEntry *next = e->next;
                    free(e);
                    e = next;
                }
            }
            free(ht->entries);
        }
        free(ht);
    }
}

size_t hash_func(const HashTable *ht, unsigned key) {
    return key % ht->cap;
}

void insert_or_update_entry(HashTable *ht, unsigned item_id, unsigned price) {
    size_t b_idx = hash_func(ht, item_id);
    TableEntry *cur = ht->entries[b_idx];
    while (cur) {
        if (cur->item_id == item_id) {
            cur->price = price;
            return;
        }
        cur = cur->next;
    }

    TableEntry *entry = new_entry(item_id, price);
    entry->next = ht->entries[b_idx];
    ht->entries[b_idx] = entry;
    ht->sz += 1;
}

#define NOT_FOUND UINT_MAX

// returns UINT_MAX if not found
unsigned get_price(const HashTable *ht, unsigned item_id) {
    size_t b_idx = hash_func(ht, item_id);
    TableEntry *cur = ht->entries[b_idx];
    while (cur) {
        if (cur->item_id == item_id) {
            return cur->price;
        }
        cur = cur->next;
    }
    return NOT_FOUND;
}

void delete_entry(HashTable *ht, unsigned item_id) {
    TableEntry **ptr = &ht->entries[hash_func(ht, item_id)];

    while (*ptr) {
        TableEntry *cur = *ptr;
        if (cur->item_id == item_id) {
            break;
        }
        ptr = &cur->next;
    }

    if (*ptr) {
        TableEntry *match = *ptr;
        *ptr = match->next;
        free(match);
        ht->sz -= 1;
    }
}

HashTable *resize_hash_table(HashTable *old, size_t new_cap) {
    assert(new_cap > 0);
    HashTable *new_ht = new_hash_table(new_cap);

    for (size_t i = 0; i < old->cap; i++) {
        TableEntry *e = old->entries[i];
        while (e) {
            TableEntry *next = e->next;

            size_t new_idx = hash_func(new_ht, e->item_id);
            e->next = new_ht->entries[new_idx];
            new_ht->entries[new_idx] = e;

            e = next;
        }
    }
    new_ht->sz = old->sz;
    free(old->entries);
    free(old);
    return new_ht;
}

void print_hash_table(const HashTable *ht) {
    printf("--- Hash table begins ---\n");
    for (size_t i = 0; i < ht->cap; i++) {
        TableEntry *e = ht->entries[i];
        while (e) {
            printf("\tItem %u => $%u\n", e->item_id, e->price);
            e = e->next;
        }
    }
    printf("--- Hash table ends ---\n");
}

int main(int argc, char **argv) {
    HashTable *ht = new_hash_table(10);

    insert_or_update_entry(ht, 111, 200);
    insert_or_update_entry(ht, 200, 400);
    insert_or_update_entry(ht, 305, 500);
    insert_or_update_entry(ht, 415, 600);
    insert_or_update_entry(ht, 415, 700);

    print_hash_table(ht);

    delete_entry(ht, 201);
    delete_entry(ht, 111);
    delete_entry(ht, 305);
    delete_entry(ht, 415);

    print_hash_table(ht);

    ht = resize_hash_table(ht, 100);
    insert_or_update_entry(ht, 305, 500);
    print_hash_table(ht);

    free_hash_table(ht);
    return 0;
}
