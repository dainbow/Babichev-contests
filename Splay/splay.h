#ifndef _SPLAY_H_
#define _SPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum RotateType {
    RIGHT,
    LEFT,
};

typedef struct node {
    int64_t key;

    struct node* parent;

    struct node* left;
    struct node* right;
} Node;

Node* CreateNode(int64_t key);
Node* DeleteNode(Node* node);
Node* DeleteTree(Node* node);

Node* Rotate(Node* head, enum RotateType type);
Node* Splay(Node* node);
Node* Merge(Node* left, Node* right);

Node* FindTree(Node* head, int64_t key);
Node* Insert(Node* root, int64_t key);
Node* Erase(Node* root, int64_t key);

int64_t Sum(Node* root, const int64_t left, const int64_t right);

void PrintTreeNodes(const Node* root, Node* curNode, FILE* output);
void MakeTreeGraph(Node* root, char* name);

#endif
