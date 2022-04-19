// #include "avl.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const uint64_t MAX_STRING_LENGTH = 21;

enum RotateType {
    RIGHT,
    LEFT,
};

enum FindType {
    INSERT,
    FIND,
};

typedef struct node {
    char* key;
    char* value;
    
    struct node* parent;
    struct node* left;
    struct node* right;

    int64_t size;
} Node;

Node* CreateNode(char* key, char* value) {
    Node* newNode = calloc(1, sizeof(Node));
    while (newNode == NULL) {
        ;
    }

    newNode->key   = key;
    newNode->value = value;
    newNode->size  = 1;

    return newNode;
}

Node* DeleteNode(Node* node) {
    if (node == NULL)
        return node;
    
    free(node->key);
    free(node->value);

    free(node);
    return NULL;
}

Node* FindTree(Node* head, char* key, enum FindType type) {
    Node* prevNode = NULL;
    
    while (head) {
        int64_t cmpResult = strncmp(key, head->key, MAX_STRING_LENGTH);
        prevNode = head;

        if (cmpResult < 0) {
            head = head->left;
        }
        else if (cmpResult == 0) {
            return head;
        }
        else {
            head = head->right;
        }
    }

    if (type == INSERT)
        return prevNode;
    
    return NULL;
}

int64_t CalculateSize(Node* node) {
    int64_t leftSize  = (node->left)  ? node->left->size  : 0;
    int64_t rightSize = (node->right) ? node->right->size : 0;

    return (leftSize > rightSize) ? (leftSize + 1) : (rightSize + 1);
}

Node* Rotate(Node* head, enum RotateType type) {
    while (head == NULL) {
        ;
    }

    Node* temp  = (type == RIGHT) ? head->left : head->right;
    while (temp == NULL) {
        ;
    }

    temp->parent = head->parent;
    if (head->parent) {
        if (head == head->parent->left) {
            head->parent->left = temp;
        }
        else {
            head->parent->right = temp;
        }
    }

    if (type == RIGHT) {
        head->left  = temp->right;

        if (temp->right)
            temp->right->parent = head;

        temp->right = head;
    }
    else {
        head->right = temp->left;
    
        if (temp->left)
            temp->left->parent = head;

        temp->left = head;
    }
    head->parent = temp;

    if (temp->right)
        temp->right->size = CalculateSize(temp->right); 
    if (temp->left)
        temp->left->size  = CalculateSize(temp->left);
    temp->size        = CalculateSize(temp);

    return temp; 
}

int64_t CalculateBalance(Node* node) {
    if (node == NULL)
        return 0;

    int64_t balance = 0;

    if (node->left)
        balance += node->left->size;
    
    if (node->right)
        balance -= node->right->size;

    return balance;
}

Node* BalanceTree(Node* balanceNode) {
    Node* preNode = NULL;
    while (balanceNode) {
        balanceNode->size = CalculateSize(balanceNode);
        int64_t balance   = CalculateBalance(balanceNode);

        switch (balance) {
            case 2: {
                if (CalculateBalance(balanceNode->left) < 0) {
                    balanceNode->left = Rotate(balanceNode->left, LEFT);
                }

                if (balanceNode->parent) {
                    if (balanceNode->parent->left == balanceNode)
                        balanceNode->parent->left = Rotate(balanceNode, RIGHT);
                    else 
                        balanceNode->parent->right = Rotate(balanceNode, RIGHT);
                }
                else {
                    return Rotate(balanceNode, RIGHT);
                }
            }
            break;

            case -2: {
                if (CalculateBalance(balanceNode->right) > 0) {
                    balanceNode->right = Rotate(balanceNode->right, RIGHT);
                }

                if (balanceNode->parent) {
                    if (balanceNode->parent->left == balanceNode)
                        balanceNode->parent->left = Rotate(balanceNode, LEFT);
                    else
                        balanceNode->parent->right = Rotate(balanceNode, LEFT);
                }
                else {
                    return Rotate(balanceNode, LEFT);
                }
            }
            break;
        
            default:
                break;
        }

        preNode = balanceNode;
        balanceNode = balanceNode->parent;
    }

    return preNode;
}

Node* EraseTree(Node* head, char* key) {
    if (head == NULL) {
        return head;
    }

    Node* findedNode = FindTree(head, key, FIND);

    if (findedNode == NULL) {
        return head;
    }
    Node* nodeParent = findedNode->parent;
    
    Node* left  = findedNode->left;
    Node* right = findedNode->right;
    if ((left == NULL) || (right == NULL)) {
        if (right) {
            right->parent = nodeParent;
        }
        if (left) {
            left->parent = nodeParent;
        }

        if (nodeParent) {
            if (nodeParent->left == findedNode) {
                nodeParent->left = (right) ? right : left;
            }
            else {
                nodeParent->right = (right) ? right : left;
            }
        }

        DeleteNode(findedNode);

        if ((left == NULL) && (right == NULL))
            return BalanceTree(nodeParent);

        if (left)
            return BalanceTree(left);

        if (right)
            return BalanceTree(right);
    }

    Node* rightNode = findedNode->right;

    while (rightNode->left) {
        rightNode = rightNode->left;
    }
    Node* balanceNode = (rightNode->parent != findedNode) ? rightNode->parent : rightNode;

    if (rightNode->parent != findedNode) {
        rightNode->parent->left = rightNode->right;
        if (rightNode->right)
            rightNode->right->parent = rightNode->parent;
    }
    else {
        rightNode->parent->right = rightNode->right;
        if (rightNode->right)
            rightNode->right->parent = rightNode->parent;
    }

    rightNode->parent = findedNode->parent;
    if (findedNode->parent) {
        if (findedNode->parent->left == findedNode) {
            findedNode->parent->left = rightNode;
        }
        else {
            findedNode->parent->right = rightNode;
        }
    }

    rightNode->left = findedNode->left;
    if (findedNode->left) {
        findedNode->left->parent = rightNode;
    }

    rightNode->right = findedNode->right;
    if (findedNode->right) {
        findedNode->right->parent = rightNode;
    }

    DeleteNode(findedNode);
    return BalanceTree(balanceNode);
}

Node* InsertTree(Node* head, char* key, char* value) {
    if (head == NULL) {
        return CreateNode(key, value);
    }
    
    Node* findedNode = FindTree(head, key, INSERT);
    int64_t cmpResult = strncmp(key, findedNode->key, MAX_STRING_LENGTH);

    if (cmpResult < 0) {
        findedNode->left = CreateNode(key, value);
        findedNode->left->parent = findedNode;

        return BalanceTree(findedNode);
    }

    if (cmpResult == 0) {
        free(findedNode->value);    
        free(key);

        findedNode->value = value;
        return head;
    }

    findedNode->right = CreateNode(key, value);
    findedNode->right->parent = findedNode;

    return BalanceTree(findedNode);
}

Node* DeleteTree(Node* node) {
    if (node == NULL)
        return node;

    if (node->left)
        DeleteTree(node->left);

    if (node->right)
        DeleteTree(node->right);

    return DeleteNode(node);
}

int main() {
    int64_t curChar = 0;
    Node* avlTree = NULL;

    // uint64_t curCommand = 0;
    while ((curChar = getchar()) != EOF) {
        // curCommand++;

        switch (curChar) {
            case 'i': {
                char* key   = calloc(MAX_STRING_LENGTH, sizeof(char));
                while (key == NULL) {
                    ;
                }

                char* value = calloc(MAX_STRING_LENGTH, sizeof(char));
                while (value == NULL) {
                    ;
                }

                scanf("nsert %21s %21s", key, value);
                avlTree = InsertTree(avlTree, key, value);
                while (avlTree == NULL) {
                    ;
                }
                // char outputName[100] = "";
                // sprintf(outputName, "./Graphs/graph%u", curCommand);
                // MakeTreeGraph(avlTree, outputName);
            }
            break;

            case 'f': {
                char* key = calloc(MAX_STRING_LENGTH, sizeof(char));
                while (key == NULL) {
                    ;
                }

                scanf("ind %21s", key);
            
                Node* findedNode = FindTree(avlTree, key, FIND);
                if (findedNode == NULL) {
                    printf("MISSING\n");
                }
                else {
                    printf("%s\n", findedNode->value);
                }

                free(key);
            }
            break;

            case 'e': {
                char* key = calloc(MAX_STRING_LENGTH, sizeof(char));
                while (key == NULL) {
                    ;
                }
                
                scanf("rase %21s", key);
                avlTree = EraseTree(avlTree, key);
                free(key);

                // char outputName[100] = "";
                // sprintf(outputName, "./Graphs/graph%u", curCommand);
                // MakeTreeGraph(avlTree, outputName);
            }
            break;

            default:
                break;
        }
    }

    avlTree = DeleteTree(avlTree);
}