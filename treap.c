#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
    uint32_t size;

    uint32_t key;
    uint32_t value;

    char* name;

    struct node* left;
    struct node* right;
} Node;

Node* CreateNode(uint32_t key, uint32_t value, char* name, Node* left, Node* right) {
    Node* newNode = calloc(1, sizeof(Node));

    newNode->size   = 1;
    
    newNode->key   = key;
    newNode->value = value;
    newNode->name  = name;

    newNode->left  = left;
    newNode->right = right;

    return newNode;
}

uint32_t GetSize(Node* node) {
    return node ? node->size : 0; 
}

void RecalcSizes(Node* node) {
    node->size = 1 + GetSize(node->left) + GetSize(node->right);
}

// void PushChange(Node* node) {
//     if (node->left) {
//         node->left->isLazy  = node->isLazy;
//     }

//     if (node->right) {
//         node->right->isLazy = node->isLazy;
//     }
    
//     node->isLazy = 0;
// }

Node* Merge(Node* left, Node* right) {
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    if (left->value > right->value) {
        left->right = Merge(left->right, right);

        RecalcSizes(left);
        return left;
    }

    right->left = Merge(left, right->left);

    RecalcSizes(right);
    return right;
}

void Split(Node* head, uint32_t size, Node** left, Node** right) {
    if (head == NULL) {
        *left  = NULL;
        *right = NULL;

        return;
    }

    Node* newLeft  = NULL;
    Node* newRight = NULL;

    uint32_t leftSize = GetSize(head->left);
    if (leftSize < size) {
        Split(head->right, size - leftSize - 1, &newLeft, &newRight);
        
        head->right = newLeft;
        RecalcSizes(head);

        *left  = head;
        *right = newRight;

        return;
    }

    Split(head->left, size, &newLeft, &newRight);
    
    head->left = newRight;
    RecalcSizes(head);

    *left  = newLeft;
    *right = head;

    return;
}

Node* Insert(Node* head, uint32_t key, uint32_t value, char* name) {
    if (head == NULL) {
        return CreateNode(key, value, name, NULL, NULL);
    }

    Node* left  = NULL;
    Node* right = NULL;
    Split(head, key - 1, &left, &right);

    Node* newNode = CreateNode(key, value, name, NULL, NULL);

    return Merge(Merge(left, newNode), right);
}

Node* Erase(Node* head, uint32_t key) {
    if (head == NULL)
        return head;

    Node* left  = NULL;
    Node* right = NULL;
    Split(head, key, &left, &right);

    Node* elemToErase = NULL;

    Split(left, key - 1, &left, &elemToErase);

    free(elemToErase->name);
    free(elemToErase);

    return Merge(left, right);
}

Node* Print(Node* head, uint32_t key) {
    if (head == NULL)
        return head;

    Node* left  = NULL;
    Node* right = NULL;
    Split(head, key, &left, &right);

    Node* elemToFind = NULL;

    Split(left, key - 1, &left, &elemToFind);
    printf("%s %u\n", elemToFind->name, elemToFind->value);

    return Merge(Merge(left, elemToFind), right);
}

Node* DeleteTree(Node* node) {
    if (node == NULL)
        return node;

    node->left  = DeleteTree(node->left);
    node->right = DeleteTree(node->right);

    free(node->name);
    free(node);

    return NULL;
}

int main() {
    uint32_t personAmount = 0;
    uint32_t rqstAmount   = 0;

    scanf("%u %u", &personAmount, &rqstAmount);

    Node* kuRevo = NULL;
    for (uint32_t curPerson = 1; curPerson <= personAmount; curPerson++) {
        char* name = calloc(31, sizeof(char));
        uint32_t salary = 0;

        scanf("%30s %u", name, &salary);

        kuRevo = Insert(kuRevo, curPerson, salary, name);
    }

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        uint32_t curCmd = 0;
        scanf("%u", &curCmd);

        switch (curCmd) {
            case 0: {
                uint32_t personNum = 0;
                char* name = calloc(31, sizeof(char));
                uint32_t salary = 0;

                scanf("%u %30s %u", &personNum, name, &salary);

                kuRevo = Insert(kuRevo, personNum, salary, name);
            }
            break;

            case 1: {
                uint32_t personNum = 0;
                scanf("%u", &personNum);

                kuRevo = Erase(kuRevo, personNum);
            }
            break;

            case 2: {
                uint32_t personNum = 0;
                scanf("%u", &personNum);

                Print(kuRevo, personNum);
            }
            break;
            
            default:
                break;
        }
    }

    kuRevo = DeleteTree(kuRevo);
}