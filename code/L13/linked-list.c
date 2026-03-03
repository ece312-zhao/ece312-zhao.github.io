#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    double val;
    struct ListNode *next;
} ListNode;

ListNode *create_list(size_t cnt) {
    ListNode *head = NULL, *cur = head;

    for (size_t i = 0; i < cnt; i++) {
        ListNode *tmp = malloc(sizeof(ListNode));
        assert(tmp);

        tmp->val = (double)i;
        tmp->next = NULL;

        if (!cur) {
            head = tmp;
        } else {
            cur->next = tmp;
        }
        cur = tmp;
    }

    return head;
}

void free_list(ListNode **head_ptr) {
    if (!head_ptr) return;

    ListNode *cur = *head_ptr;
    while (cur) {
        ListNode *next = cur->next;
        free(cur);
        cur = next;
    }
    *head_ptr = NULL;
}

void print_list(ListNode *head) {
    ListNode *cur = head;
    while (cur) {
        printf("%.1f ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

ListNode *find_in_list(ListNode *head, double target) {
    ListNode *cur = head;
    while (cur && cur->val != target) {
        cur = cur->next;
    }
    return cur;
}

void insert_after(ListNode *elem, double val) {
    if (!elem) return;

    ListNode *node = malloc(sizeof(ListNode));
    assert(node);
    node->val = val;
    node->next = elem->next;
    elem->next = node;
}

void delete_elem(ListNode **head_ptr, ListNode *elem) {
    if (!elem || !head_ptr || !(*head_ptr)) return;

    if (*head_ptr == elem) {
        *head_ptr = elem->next;
    } else {
        ListNode *parent = *head_ptr;
        while (parent && parent->next != elem) {
            parent = parent->next;
        }

        if (!parent) return;
        parent->next = elem->next;
    }
    free(elem);
}

// https://www.youtube.com/watch?v=78Y17hAo96I
void delete_elem_2(ListNode **head_ptr, ListNode *elem) {
    if (!elem || !head_ptr || !*head_ptr) return;

    ListNode **cur_next = head_ptr;
    while (*cur_next && *cur_next != elem) cur_next = &((*cur_next)->next);

    if (!(*cur_next)) return;

    *cur_next = elem->next;
    free(elem);
}

int main() {
    ListNode *list = create_list(10);

    print_list(list);

    ListNode *elem = find_in_list(list, 5.0);
    insert_after(elem, 5.5);
    print_list(list);

    delete_elem(&list, elem);
    print_list(list);

    printf("head=%p\n", (void *)list);
    delete_elem(&list, list);
    print_list(list);
    printf("head=%p\n", (void *)list);

    free_list(&list);

    return 0;
}
