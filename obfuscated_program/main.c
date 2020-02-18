#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * This program basically divides input into 32-bit
 * unsigned integers, fills XOR-linked list with them
 * and performs bubble sort on it.
 * More on XOR-linked list here:
 * https://en.wikipedia.org/wiki/XOR_linked_list
 */

// to be compiled under x86

struct node {
    uint32_t data;
    struct node* npx; /* XOR of next and previous node */
};

/* returns XORed value of the node addresses */
#define XOR(a, b) ((struct node*) ((uint32_t) (a) ^ (uint32_t) (b)))
#define XORSWAP(a, b)  ((a) ^= (b), (b) ^= (a), (a) ^= (b))

// Print contents of doubly linked list in forward direction
void printList(struct node *head) {
    struct node *curr = head;
    struct node *prev = NULL;
    struct node *next;
    printf("Following are the nodes of Linked List: \n");
    while (curr != NULL) {
        // print current node
        printf("%u ", curr->data);
        // get address of next node: curr->npx is next^prev, so curr->npx^prev
        // will be next^prev^prev which is next
        next = XOR(prev, curr->npx);
        // update prev and curr for next iteration
        prev = curr;
        curr = next;
    }
    printf("\n");
}

/* Insert a node at the begining of the XORed linked list and makes the
   newly inserted node as head */
void insert(struct node **head_ref, struct node* new_node, uint32_t data) {
    new_node->data = data;
    /* Since new node is being inserted at the begining, npx of new node
       will always be XOR of current head and NULL */
    new_node->npx = *head_ref;
    /* If linked list is not empty, then npx of current head node will be XOR
       of new node and node next to current head */
    if (*head_ref != NULL) {
        // *(head_ref)->npx is XOR of NULL and next. So if we do XOR of
        // it with NULL, we get next
        struct node* next = (*head_ref)->npx;
        (*head_ref)->npx = XOR(new_node, next);
    }
    // Change head
    *head_ref = new_node;
}

void function(const char* input, uint32_t len) {
    struct node *some_space = (struct node *)malloc((sizeof(struct node) / 4) * len);

    struct node *head = NULL;

    for (uint32_t i = 0; i < len / 4; i++) {
        insert(&head, &some_space[i], ((uint32_t*)input)[i]);
    }

    for (uint32_t i = 0; i < len / 4-1; i++) {
        struct node *prev = NULL;
        struct node *cur  = &some_space[0];
        for (uint32_t j = 0; j < len/4-i-1; j++) {
            if (cur->data > XOR(prev, cur->npx)->data)
                XORSWAP(XOR(prev, cur->npx)->data, cur->data);
            struct node *new  = XOR(prev, cur->npx);
            prev = cur;
            cur  = new;
        }
    }

    printList(head);
}

int main() {
    int input[] = {-1, 155, 16};
    function(input, sizeof(input));
    return (0);
}

