#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "queue.h"

typedef void *pointer;

struct BitCoin {
    unsigned long int bid;
    bcNode root;
    unsigned long int nodes;
};

struct Node {
    struct Transaction *transaction;
    char *walletId;
    unsigned long int balance;
    bcNode parent, left, right;
};


/***Private functions***/

bool _isLeaf(bcNode node) {
    return node->left == NULL && node->right == NULL;
}

bool _isTarget(bcNode node, char *walletId) {
    return !strcmp(node->walletId, walletId);
}

bcNode _createNode(bcNode parent, unsigned long int balance, char *walletId, Transaction transaction) {
    bcNode node = (bcNode) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->parent = parent;
        node->balance = balance;
        node->walletId = malloc(strlen(walletId) * sizeof(char) + 1);
        strcpy(node->walletId, walletId);
        node->transaction = transaction;
        node->right = NULL;
        node->left = NULL;
        return node;
    } else
        return NULL;
}

void _destroyNode(bcNode bc) {
    fprintf(stdout, "Bitcoin node to be destroyed: [%p]\n", bc);
    free(bc);
}

/***Public functions***/

void bcTrace(bitcoin bc) {
    assert(bc != NULL);
    Queue queue = NULL;
    queueCreate(&queue, bc->nodes);
    bcNode root = bc->root;

    printf("Trace transactions for bitcoin %lu with initial owner '%s' and initial balance: %lu$\n",
           bc->bid, bc->root->walletId, root->balance);

    while (root != NULL) {
        transactionPrint(root->transaction);
        if (root->left != NULL) {
            enQueue(queue, root->left);
        }
        if (root->right != NULL) {
            enQueue(queue, root->right);
        }
        root = deQueue(queue);
    }

    queueDestroy(&queue);

    //printf("\n", transactions);
}


bool bcInsert(bitcoin bc, unsigned long int *rest, Transaction transaction) {
    assert(bc != NULL);
    assert(*rest > 0);
    assert(transaction != NULL);

    Queue queue = NULL;

    queueCreate(&queue, bc->nodes);

    bcNode sender = bc->root;
    while (sender != NULL) {

        printf("BID: '%lu'\tNODE: [%p]\tBalance: %lu$\n", bc->bid, sender, sender->balance);

        if (sender->left != NULL) {
            enQueue(queue, sender->left);
        }

        if (sender->right != NULL) {
            enQueue(queue, sender->right);
        }

        if (_isLeaf(sender) && _isTarget(sender, transaction->senderWalletId)) {
            printf("Target leaf for walletId '%s' is [%p]\n", sender->walletId, sender);

            if (*rest < sender->balance) {

                /* Left node for receiver*/
                sender->left = _createNode(sender, *rest, transaction->receiverWalletId, transaction);
                bc->nodes++;
                printf("Create left child [%p] with amount %lu$\n", sender->left, *rest);
                //todo: insert this bit coin in receiver's wallet list only if not already exists!!!


                /* Right node for sender's rest*/
                sender->right = _createNode(sender, sender->balance - *rest, transaction->senderWalletId, transaction);
                bc->nodes++;

                printf("Create right child [%p] with amount %lu$\n", sender->right, sender->balance - *rest);
                *rest = 0;
                break;
            } else if (*rest >= sender->balance) {

                /* Left node for receiver*/
                sender->left = _createNode(sender, sender->balance, transaction->receiverWalletId, transaction);
                bc->nodes++;

                printf("Create left child [%p] with amount %lu$\n", sender->left, sender->balance);
                //todo: insert this bit coin in receiver's wallet list only if not already exists!!!

                //todo: if user of this bitcoin does not have any other value, then remove this bitcoin from
                // sender's list'


                *rest -= sender->balance;
            }
            printf(" ");
        }
        sender = deQueue(queue);
    }

    queueDestroy(&queue);

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
        bc->nodes = 1;
        bc->bid = htBitCoinParams->bid;
        bc->root = _createNode(NULL, htBitCoinParams->v, htBitCoinParams->walletId, NULL);
        if (bc->root == NULL) {
            bcDestroy(bc);
            return NULL;
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
    Queue queue = NULL;
    queueCreate(&queue, bc->nodes);
    bcNode root = bc->root;
    while (root != NULL) {
        printf("\nNode to be destroyed: [%p] b:[%lu]\n", root, root->balance);
        _destroyNode(root);

        if (root->left != NULL) {
            enQueue(queue, root->left);
        }

        if (root->right != NULL) {
            enQueue(queue, root->right);
        }
        root = deQueue(queue);
    }
    queueDestroy(&queue);
    free(bc);
}
