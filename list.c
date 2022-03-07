#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct list {
    int32_t data;

    struct list* next;
};

struct list* list_new(int elem) {
    struct list* newList = (struct list*)calloc(1, sizeof(struct list));
    if (newList == NULL)
        return NULL;

    newList->data = elem;
    newList->next = NULL;

    return newList;
}

struct list* list_insert(struct list* head, int elem) {
    if (head == NULL) {
        return list_new(elem);
    }

    struct list* rememberHead = head;

    while (head->next != NULL) {
        head = head->next;
    }

    head->next = list_new(elem);

    return rememberHead;
}

struct list* list_find (struct list* head, int elem) {
    if (head == NULL) {
        return head;
    }

    while (1) {
        if (head->data == elem)
            return head;

        if (head->next != NULL)
            head = head->next;
        else
            break;
    }

    return NULL;
}

struct list* list_erase (struct list* head, int elem) {
    if (head == NULL)
        return NULL;

    if (head->data == elem) {
        struct list* nodeToRet = head->next;
        
        free(head);
        return nodeToRet;
    }

    struct list* rememberHead = head;

    while (head->next != NULL) {
        if (head->next->data == elem) {
            struct list* listToDel  = head->next;
            head->next              = head->next->next;

            free(listToDel);
            return rememberHead;
        }

        head = head->next;
    }
 
    return rememberHead;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what) {
    if ((where == NULL) || (what == NULL))
        return head;
    
    if (head == NULL)
        return what;

    struct list* rememberHead = head;
    
    while (1) {
        if (head == where) {
            what->next = head->next;
            head->next = what;

            return rememberHead;
        }

        if (head->next != NULL)
            head = head->next;
        else
            break;
    }

    return NULL;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what) {    
    if ((where == NULL) || (what == NULL))
        return head;
    
    if (head == NULL)
        return what;
    
    if (where == head) {
        what->next = where;
        return what;
    }

    struct list* rememberHead = head;

    while (head->next != NULL) {
        if (head->next == where) {
            head->next = what;
            what->next = where;

            return rememberHead;
        }
     

        head = head->next;
    }

    return NULL;
}

struct list* list_delete(struct list* head) {
    if (head == NULL)
        return NULL;
    
    while(head->next != NULL) {
        struct list* prevNode = head;

        head = head->next;
        free(prevNode);
    }

    free(head);
    return NULL;
}

struct list *list_next(struct list *curr) {
    if (curr == NULL)
        return NULL;

    return curr->next;
}

void list_print(struct list const *head) {
    printf("[");

    if (head != NULL) {
        while (head->next != NULL) {
            printf("%d, ", head->data);

            head = head->next;
        }

        printf("%d", head->data);
    }

    printf("]\n");
}

// int main() {
//     struct list *head = list_new(0);
//     head = list_insert(head, 1);
//     head = list_insert(head, 2);
//     head = list_insert(head, 4);
//     head = list_erase(head, 4);
//     head = list_erase(head, 2);
//     head = list_erase(head, 1);
//     head = list_erase(head, 0);
//     head = list_new(17);
//     head = list_insert_before(head, list_find(head, 17), list_new(13));
//     head = list_insert_after(head, list_find(head, 13), list_new(22));
//     head = list_delete(head);
//     head = list_new(8);
//     head = list_insert_after(head, list_find(head, 8), list_new(10));

//     printf("0 is at %p\n", list_find(head, 0));

//     head = list_insert(head, 2);
//     head = list_insert_before(head, list_find(head, 2), list_new(228));

//     list_print(head);
//     head = list_delete(head);
//     list_print(head);
// }
