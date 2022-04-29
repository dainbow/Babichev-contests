#ifndef _SPLAY_H_
#define _SPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct node {
    int32_t key;
    int64_t sum;

    struct node* parent;

    struct node* left;
    struct node* right;
} Node;

Node* CreateNode(int32_t key);
Node* DeleteNode(Node* node);

static inline void RotateLeft(Node* head);
static inline void RotateRight(Node* head);

static inline Node* Splay(Node* node);
static inline Node* FindTree(Node* head, int32_t key);

static inline Node* GetMax(Node* root);
static inline Node* Join(Node* less, Node* greater);

static inline void Split(Node* root, int32_t key, Node** left, Node** right, Node** middle);
static inline Node* Insert(Node* root, int32_t key);

Node* Merge(Node* left, Node* right);
Node* Erase(Node* root, int32_t key);

Node* DeleteTree(Node* node);

void PrintTreeNodes(const Node* root, Node* curNode, FILE* output);
void MakeTreeGraph(Node* root, char* name);

#endif
