#ifdef G_DEBUD
#include "splay.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #pragma GCC optimize("O3")

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

#endif

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
                int32_t left  = 0;
                int32_t right = 0;

                scanf("%d %d", &left, &right);
                
                Node* lessLeft  = NULL;
                Node* grtrLeft  = NULL;
                Node* eqLeft    = NULL;

                Node* sumTree   = NULL;
                Node* eqRight   = NULL;
                Node* grtrRight = NULL;

                Split(splayTree, left, &lessLeft, &grtrLeft, &eqLeft);
                sumValue  = (eqLeft != NULL) * left;

                Split(grtrLeft, right, &sumTree,  &grtrRight, &eqRight);
                sumValue += (eqRight != NULL) * right;

                if (sumTree)
                    sumValue += sumTree->sum;
                
                printf("%ld\n", sumValue);

                splayTree = Join(Join(Join(lessLeft, eqLeft), sumTree) , Join(eqRight, grtrRight));
                
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
                int32_t key = 0;
                scanf("%d", &key);

                if (sumFlag) {
                    splayTree = Insert(splayTree, ((int64_t)key + sumValue) % 1000000000);
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

Node* CreateNode(int32_t key) {
    Node* newNode = calloc(1, sizeof(Node));

    newNode->key   = key;
    newNode->sum   = key;

    return newNode;
}

Node* DeleteNode(Node* node) {
    if (node == NULL)
        return node;
    
    free(node);
    return NULL;
}

static inline void RotateLeft(Node* head) {
    head->sum       = head->key + ((head->left) ? head->left->sum : 0) + 
                                  ((head->right->left) ? head->right->left->sum : 0);

    head->right->sum = head->sum + ((head->right->right) ? head->right->right->sum : 0) + head->right->key; 

    Node* temp  = head->right;

    temp->parent = head->parent;
    if (head->parent) {
        if (head == head->parent->left) {
            head->parent->left = temp;
        }
        else {
            head->parent->right = temp;
        }
    }
    
    head->right = temp->left;

    if (temp->left)
        temp->left->parent = head;

    temp->left = head;
    head->parent = temp;
}

static inline void RotateRight(Node* head) {
    head->sum       = ((head->right) ? head->right->sum : 0) + 
                      ((head->left->right) ? head->left->right->sum : 0) + head->key;

    head->left->sum = head->sum + ((head->left->left) ? head->left->left->sum : 0) + head->left->key; 

    Node* temp  = head->left;

    temp->parent = head->parent;
    if (head->parent) {
        if (head == head->parent->left) {
            head->parent->left = temp;
        }
        else {
            head->parent->right = temp;
        }
    }

    head->left  = temp->right;

    if (temp->right)
        temp->right->parent = head;

    temp->right = head;
    head->parent = temp;
}

static inline Node* Splay(Node* node) {
    while (node->parent) {
        if (node->parent->parent == NULL) {
            if (node == node->parent->left) {
                RotateRight(node->parent);
            }
            else {
                RotateLeft(node->parent);
            }
        }
        else {
            if ((node == node->parent->left) && (node->parent == node->parent->parent->left)) {
                RotateRight(node->parent->parent);
                RotateRight(node->parent);
            }
            else if ((node == node->parent->right) && (node->parent == node->parent->parent->right)) {
                RotateLeft(node->parent->parent);
                RotateLeft(node->parent);
            }
            else if ((node == node->parent->right) && (node->parent == node->parent->parent->left)) {
                RotateLeft(node->parent);
                RotateRight(node->parent);
            }
            else {
                RotateRight(node->parent);
                RotateLeft(node->parent);
            }
        }
    }

    return node;
}

static inline Node* FindTree(Node* head, int32_t key) {
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

    if (preNode == NULL)
        return preNode;

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
    newRoot->sum += greater->sum;
    
    greater->parent = newRoot;

    return newRoot;
}

static inline void Split(Node* root, int32_t key, Node** left, Node** right, Node** middle) {
    if (root == NULL)
        return;

    root = FindTree(root, key);

    if (root->key < key) {
        *right = root->right;
        *left  = root;

        if (root->right) {
            root->right->parent = NULL;
            root->sum -= root->right->sum;
        }
        root->right = NULL; 

        return;
    }

    if (root->key > key) {
        *right = root;
        *left  = root->left;
        
        if (root->left) {
            root->left->parent = NULL;
            root->sum -= root->left->sum;
        }
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
    root->sum = root->key;

    return;
}

static inline Node* Insert(Node* root, int32_t key) {
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
    if (left) {
        left->parent = newNode;
        newNode->sum += left->sum;
    }

    newNode->right = right;
    if (right) {
        right->parent = newNode;
        newNode->sum += right->sum;
    }

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

Node* Erase(Node* root, int32_t key) {
    root = FindTree(root, key);

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

Node* DeleteTree(Node* node) {
    if (node == NULL)
        return node;

    if (node->left)
        DeleteTree(node->left);

    if (node->right)
        DeleteTree(node->right);

    return DeleteNode(node);
}
