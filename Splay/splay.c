#ifdef G_DEBUD
#include "splay.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #pragma GCC optimize("O3")

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

// Node* CreateNode(int64_t key);
// Node* DeleteNode(Node* node);
// Node* DeleteTree(Node* node);

// Node* Rotate(Node* head, enum RotateType type);
// static inline Node* Splay(Node* node);

// Node* Merge(Node* left, Node* right);
// static inline void Split(Node* root, int64_t key, Node** left, Node** right, Node** middle);
// static inline Node* Join(Node* less, Node* greater);

// static inline Node* FindTree(Node* head, int64_t key);
// Node* Insert(Node* root, int64_t key);
// Node* Erase(Node* root, int64_t key);

// int64_t Sum(Node* root);
#endif

Node* CreateNode(int64_t key) {
    Node* newNode = calloc(1, sizeof(Node));

    newNode->key   = key;

    return newNode;
}

Node* DeleteNode(Node* node) {
    if (node == NULL)
        return node;
    
    free(node);
    return NULL;
}

Node* Rotate(Node* head, enum RotateType type) {
    Node* temp  = (type == RIGHT) ? head->left : head->right;

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

    return temp; 
}

static inline Node* Splay(Node* node) {
    if (node == NULL)
        return NULL;

    while (node->parent) {
        if (node->parent->parent == NULL) {
            if (node == node->parent->left) {
                Rotate(node->parent, RIGHT);
            }
            else {
                Rotate(node->parent, LEFT);
            }
        }
        else {
            if ((node == node->parent->left) && (node->parent == node->parent->parent->left)) {
                Rotate(node->parent->parent, RIGHT);
                Rotate(node->parent, RIGHT);
            }
            else if ((node == node->parent->right) && (node->parent == node->parent->parent->right)) {
                Rotate(node->parent->parent, LEFT);
                Rotate(node->parent, LEFT);
            }
            else if ((node == node->parent->right) && (node->parent == node->parent->parent->left)) {
                Rotate(node->parent, LEFT);
                Rotate(node->parent, RIGHT);
            }
            else {
                Rotate(node->parent, RIGHT);
                Rotate(node->parent, LEFT);
            }
        }
    }

    return node;
}

static inline Node* FindTree(Node* head, int64_t key) {
    Node* preNode = NULL;

    while (head) {
        preNode = head;
        if (key < head->key) {
            head = head->left;
        }
        else if (key == head->key) {
            return Splay(head);
        }
        else {
            head = head->right;
        }
    }

    return Splay(preNode);
}

static inline Node* GetMax(Node* root) {
    if (root == NULL)
        return root;

    while (root->right)
        root = root->right;

    return root;
}

static inline Node* Join(Node* less, Node* greater) {
    if (less == NULL)
        return greater;
    
    if (greater == NULL)
        return less;

    Node* newRoot = GetMax(less);
    newRoot = Splay(newRoot);

    newRoot->right = greater;
    greater->parent = newRoot;

    return newRoot;
}

static inline void Split(Node* root, int64_t key, Node** left, Node** right, Node** middle) {
    if (root == NULL)
        return;

    root = FindTree(root, key);

    if (root->key < key) {
        *right = root->right;
        *left  = root;

        if (root->right)
            root->right->parent = NULL;
        root->right = NULL; 

        return;
    }

    if (root->key > key) {
        *right = root;
        *left  = root->left;
        
        if (root->left)
            root->left->parent = NULL;
        root->left = NULL;

        return;
    }

    if (root->left)
        root->left->parent = NULL;

    if (root->right)
        root->right->parent = NULL;

    *left   = root->left;
    root->left = NULL;

    *right  = root->right;
    root->right = NULL;

    *middle = root;

    return;
}

Node* Insert(Node* root, int64_t key) {
    Node* left  = NULL;
    Node* right = NULL;
    Node* middle = NULL;

    Split(root, key, &left, &right, &middle);

    if (middle) {
        middle->left = left;
        if (left)
            left->parent = middle;

        middle->right = right;
        if (right)
            right->parent = middle;

        return middle;
    }

    Node* newNode = CreateNode(key);
    newNode->left = left;
    if (left)
        left->parent = newNode;

    newNode->right = right;
    if (right)
        right->parent = newNode;

    return newNode;
}

Node* Merge(Node* left, Node* right) {
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    left = FindTree(left, right->key);

    left->right   = right;
    right->parent = left;

    return left;
}

Node* Erase(Node* root, int64_t key) {
    root = Splay(FindTree(root, key));

    if (root->key != key) {
        return root;
    }

    Node* left  = root->left;
    Node* right = root->right;

    DeleteNode(root);

    if (left) {
        left->parent = NULL;
    }

    if (right) {
        right->parent = NULL;
    }

    return Merge(left, right);
}

int64_t Sum(Node* root) {
    int64_t asnwer = root->key;

    if (root->left) {
        asnwer += Sum(root->left);
    }

    if (root->right) {
        asnwer += Sum(root->right);
    }

    return asnwer;
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
    Node* splayTree = NULL;

    uint32_t rqstAmount = 0;
    scanf("%u", &rqstAmount);

    int8_t sumFlag   = 0;
    int64_t sumValue = 0;

    #ifdef G_DEBUG
    uint32_t insertID = 0;
    #endif

    for (uint32_t curRqst = 0; curRqst < rqstAmount; curRqst++) {
        int32_t curChar = getchar();

        switch (curChar) {
            case '?': {
                int64_t left  = 0;
                int64_t right = 0;

                scanf("%ld %ld", &left, &right);

                if (splayTree != NULL) {
                    Node* lessLeft  = NULL;
                    Node* grtrLeft  = NULL;
                    Node* eqLeft    = NULL;

                    Node* sumTree   = NULL;
                    Node* eqRight   = NULL;
                    Node* grtrRight = NULL;

                    Split(splayTree, left, &lessLeft, &grtrLeft, &eqLeft);
                    sumValue = (eqLeft != NULL) * left;

                    Split(grtrLeft, right, &sumTree,  &grtrRight, &eqRight);
                    sumValue += (eqRight != NULL) * right;

                    if (sumTree)
                        sumValue += Sum(sumTree);
                    
                    printf("%ld\n", sumValue);

                    splayTree = Join(lessLeft, eqLeft);
                    splayTree = Join(splayTree, sumTree);
                    splayTree = Join(splayTree, eqRight);
                    splayTree = Join(splayTree, grtrRight);
                }
                else {
                    printf("0\n");
                }
                sumFlag = 1;

                #ifdef G_DEBUG
                insertID++;

                char graphName[100] = "";
                sprintf(graphName, "./Graphs/graph%d", insertID);
                MakeTreeGraph(splayTree, graphName);
                #endif
            }
            break;

            case '+': {
                int64_t key = 0;
                scanf("%ld", &key);

                if (sumFlag) {
                    splayTree = Insert(splayTree, (key + sumValue) % 1000000000);
                }
                else {
                    splayTree = Insert(splayTree, key);
                }
                sumFlag = 0;

                #ifdef G_DEBUG
                insertID++;

                char graphName[100] = "";
                sprintf(graphName, "./Graphs/graph%d", insertID);
                MakeTreeGraph(splayTree, graphName);
                #endif
            }
            break;


            default:
                curRqst--;
                break;
        }
    }

    splayTree = DeleteTree(splayTree);
}
