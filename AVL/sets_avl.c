// #include "sets_avl.h"
// #include "avl.h"

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
    uint64_t key;
    
    struct node* parent;
    struct node* left;
    struct node* right;

    int64_t size;
} Node;

Node* CreateNode(uint64_t key);
Node* DeleteNode(Node* node);
Node* DeleteTree(Node* node);

Node* FindTree(Node* head, uint64_t key, enum FindType type);

int64_t CalculateSize(Node* node);
int64_t CalculateBalance(Node* node);

Node* Rotate(Node* head, enum RotateType type);
Node* BalanceTree(Node* balanceNode);

Node* EraseTree(Node* head, uint64_t key);
Node* InsertTree(Node* head, uint64_t key);

void PrintTree(Node* node);

int main() {
    uint32_t treeAmount    = 0;
    uint32_t requestAmount = 0;

    scanf("%u %u", &treeAmount, &requestAmount);
    treeAmount++;

    Node** treeArr = calloc(treeAmount, sizeof(Node*));

    int32_t rqstType = 0;
    for (uint32_t curRqst = 0; curRqst < requestAmount; curRqst++) {
        rqstType = getchar();

        switch(rqstType) {
            case '+': {
                uint32_t treeNumber = 0;
                uint64_t key = 0;

                scanf("%lu %u", &key, &treeNumber);
                treeArr[treeNumber] = InsertTree(treeArr[treeNumber], key);
            }
            break;

            case '-': {
                uint32_t treeNumber = 0;
                uint64_t key = 0;

                scanf("%lu %u", &key, &treeNumber);
                treeArr[treeNumber] = EraseTree(treeArr[treeNumber], key);
            }
            break;

            case '=': {
                uint32_t treeNumber = 0;

                scanf("%u", &treeNumber);
                treeArr[treeNumber] = DeleteTree(treeArr[treeNumber]);
            }
            break;

            case '?': {
                if (getchar() != '?') {
                    uint32_t treeNumber = 0;

                    scanf("%u", &treeNumber);

                    if (treeArr[treeNumber] == NULL) {
                        printf("-1\n");
                    }
                    else {
                        PrintTree(treeArr[treeNumber]);
                        printf("\n");
                    }
                }
                else {
                    uint64_t key = 0;
                    scanf("%lu", &key);

                    char isOneFlag = 0;

                    for (uint32_t curTree = 0; curTree < treeAmount; curTree++) {
                        Node* findedNode = FindTree(treeArr[curTree], key, FIND);

                        if (findedNode) {
                            printf("%u ", curTree);
                            isOneFlag = 1;
                        }
                    }

                    if (isOneFlag == 0) {
                        printf("-1");
                    }
                    printf("\n");
                }
            }
            break;

            default:
                curRqst--;
                break;
        }
    }

    for (uint32_t curTree = 0; curTree < treeAmount; curTree++) {
        treeArr[curTree] = DeleteTree(treeArr[curTree]);
    }
}

Node* CreateNode(uint64_t key) {
    Node* newNode = calloc(1, sizeof(Node));
    while(newNode == NULL) {
        ;
    }

    newNode->key   = key;
    newNode->size  = 1;

    return newNode;
}

Node* DeleteNode(Node* node) {
    if (node == NULL)
        return node;

    free(node);
    return NULL;
}

Node* FindTree(Node* head, uint64_t key, enum FindType type) {
    Node* prevNode = NULL;
    
    while (head) {
        int64_t cmpResult = 0;
        if (key < head->key)
            cmpResult = -1;
        else if (key > head->key)
            cmpResult = 1;

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

Node* EraseTree(Node* head, uint64_t key) {
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

Node* InsertTree(Node* head, uint64_t key) {
    if (head == NULL) {
        return CreateNode(key);
    }
    
    Node* findedNode = FindTree(head, key, INSERT);
    int64_t cmpResult = 0;
    if (key < findedNode->key)
        cmpResult = -1;
    else if (key > findedNode->key)
        cmpResult = 1;

    if (cmpResult < 0) {
        findedNode->left = CreateNode(key);
        findedNode->left->parent = findedNode;

        return BalanceTree(findedNode);
    }

    if (cmpResult == 0) {
        return head;
    }

    findedNode->right = CreateNode(key);
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

void PrintTree(Node* node) {
    if (node->left)
        PrintTree(node->left);

    printf("%lu ", node->key);

    if (node->right)
        PrintTree(node->right);
}
