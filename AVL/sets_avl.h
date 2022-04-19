#ifndef _SETS_AVL_H_
#define _SETS_AVL_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum RotateType {
    RIGHT,
    LEFT,
};

enum FindType {
    INSERT,
    FIND,
};

typedef struct node {
    int64_t key;
    
    struct node* parent;
    struct node* left;
    struct node* right;

    int64_t size;
} Node;

Node* CreateNode(int64_t key);
Node* DeleteNode(Node* node);
Node* DeleteTree(Node* node);

Node* FindTree(Node* head, int64_t key, enum FindType type);

int64_t CalculateSize(Node* node);
int64_t CalculateBalance(Node* node);

Node* Rotate(Node* head, enum RotateType type);
Node* BalanceTree(Node* balanceNode);

Node* EraseTree(Node* head, int64_t key);
Node* InsertTree(Node* head, int64_t key);

#endif
