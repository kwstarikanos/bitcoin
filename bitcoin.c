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

bcNode _createNode(bcNode parrent) {
    bcNode node = (bcNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->parrent = parrent;
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}


/***Public functions***/
bool bcInsert(bitcoin bc, bcNode l, bcNode r, Transaction transaction) {
    assert(bc != NULL);
    assert(transaction != NULL);



    _createNode(NULL);


    return true;
}

long unsigned int bcGetId(bitcoin bc) {
    return bc->bid;
}

void bcDestroyNode(bcNode bc){
    if(bc !=NULL){
        printf("\nbc node to be destroyed: [%p]\n", bc);
    }
}

/* @Callback
 * Initialize & return a new BitCoin*/
bitcoin bcCreate(ht_bitcoin_params *htBitCoinParams) {
    bitcoin bc = NULL;
    bc = (bitcoin) malloc(sizeof(struct BitCoin));
    if (bc != NULL) {
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode(NULL);
        bc->root->wallet = htBitCoinParams->wallet;
        bc->root->balance = htBitCoinParams->v;
    }
    return bc;
}

/* @Callback
 * Compare trees function for bitcoins hashtable*/
int bcCompare(bitcoin bc1, bitcoin bc2) {
    return bc1->bid != bc2->bid;
}

/* @Callback
 * Hash function for bitcoins hashtable*/
unsigned long int bitcoinHash(const long int *bid, unsigned long int capacity) {
    return *bid % capacity;
}

/* @Callback
 * Compare keys function for wallets hashtable*/
void bcDestroy(bitcoin *bc) {
    printf("[%p]", *bc);
}
