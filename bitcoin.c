#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef void *pointer;

struct BitCoin {
    unsigned long int bid;
    bcNode root;
};

struct Node {
    struct Transaction *transaction;
    Wallet wallet;
    unsigned long int balance;
    bcNode parrent, left, right;
};

/***Private functions***/

bcNode _createNode() {
    bcNode node = (bcNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->parrent = NULL;
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}

void _destroyNode(bcNode bc) {
    if (bc != NULL) {
        fprintf(stdout, "Bitcoin node to be destroyed: [%p]\n", bc);
        free(bc);
    }
}

/* Given a binary tree, print its nodes in level order
   using array for implementing queue *//*

void printLevelOrder(struct node* root)
{
    int rear, front;
    struct node **queue = createQueue(&front, &rear);
    struct node *temp_node = root;

    while (temp_node)
    {
        printf("%d ", temp_node->data);

        */
/*Enqueue left child *//*

        if (temp_node->left)
            enQueue(queue, &rear, temp_node->left);

        */
/*Enqueue right child *//*

        if (temp_node->right)
            enQueue(queue, &rear, temp_node->right);

        */
/*Dequeue node and make it temp_node*//*

        temp_node = deQueue(queue, &front);
    }
}
*/


/***Public functions***/
bool bcInsert(bitcoin bc, unsigned long int *rest, Transaction transaction) {
    assert(bc != NULL);
    assert(*rest > 0);
    assert(transaction != NULL);


    // *p = _createNode(NULL);
    return true;
}

long unsigned int bcGetId(bitcoin bc) {
    return bc->bid;
}

/* @Callback
 * Initialize & return a new BitCoin*/
bitcoin bcCreate(ht_bitcoin_params *htBitCoinParams) {
    bitcoin bc = NULL;
    bc = (bitcoin) malloc(sizeof(struct BitCoin));
    if (bc != NULL) {
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode();
        if (bc->root != NULL) {
            bc->root->transaction = NULL;
            bc->root->wallet = htBitCoinParams->wallet;
            bc->root->balance = htBitCoinParams->v;
        }
    }
    return bc;
}

/* @Callback
 * Compare bitcoins*/
int bcCompare(bitcoin bc1, bitcoin bc2) {
    return bc1->bid != bc2->bid;
}

/* @Callback
 * Hash bitcoin*/
unsigned long int bitcoinHash(const long int *bid, unsigned long int capacity) {
    return *bid % capacity;
}

/* @Callback
 * Destroy bitcoin*/
void bcDestroy(bitcoin bc) {
    assert(bc != NULL);
    printf("\nbcDestroy: [%p] ", bc);

    //TODO: BFS to destroy all nodes!
    _destroyNode(bc->root);

    
    free(bc);
}
