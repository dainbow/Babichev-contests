#ifndef _AVL_H_
#define _AVL_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum FindType {
    INSERT,
    FIND,
};

typedef struct node {
    char* key;
    char* value;
    
    int32_t size;
    struct node* parent;
    struct node* left;
    struct node* right;
} Node;

void PrintTreeNodes(const Node* root, Node* curNode, FILE* output);
void MakeTreeGraph(Node* root, char* name);

#endif
