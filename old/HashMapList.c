#include "IntMapList.h"

#include <stdio.h>
#include <stdlib.h>

#include "DynamicString.h"

IntMapNode *IntMapNode_new(int key, int val, IntMapNode *prev, IntMapNode *next) {
    IntMapNode *node = (IntMapNode *)malloc(sizeof(IntMapNode));
    if (node != NULL) {
        node->key = key;
        node->val = val;
        node->prev = prev;
        node->next = next;
    }
    return node;
}

void IntMapNode_free(IntMapNode *node) {
    free(node);
}

IntMapList *IntMapList_new(void) {
    IntMapList *list = (IntMapList *)malloc(sizeof(IntMapList));
    if (list != NULL) {
        list->len = 0;
    }
    return list;
}

void IntMapList_free(IntMapList *list) {
    IntMapNode *curr = list->head;
    while (curr != NULL) {
        IntMapNode *next = curr->next;
        IntMapNode_free(curr);
        curr = next;
    }
    free(list);
}

IntMapNode *IntMapList_put(IntMapList *list, int key, int val) {
    IntMapNode *node = list->head;
    // check that the list has a node
    if (node == NULL) {
        list->head = IntMapNode_new(key, val, NULL, NULL);
        list->tail = list->head;
        list->len++;
        return list->head;
    }
    // search for the key
    while (node->next != NULL) {
        if (node->key == key) {
            node->val = val;
            return node;
        }
        node = node->next;
    }
    if (node->key == key) {
        node->val = val;
        return node;
    }
    // otherwise add the node
    node->next = IntMapNode_new(key, val, NULL, NULL);
    node->next->prev = node;
    list->tail = node->next;
    list->len++;
    return node->next;
}

IntMapNode *IntMapList_find(IntMapList *list, int key) {
    IntMapNode *node = list->head;
    // search for the key
    while (node != NULL) {
        if (node->key == key) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void IntMapList_remove(IntMapList *list, int key) {
    IntMapNode *node = IntMapList_find(list, key);
    if (node == NULL) {
        return;
    }

    IntMapNode *prev = node->prev;
    IntMapNode *next = node->next;
    
    if (prev == NULL) {
        list->head = next;
    }
    else {
        prev->next = next;
    }

    if (next == NULL) {
        list->tail = prev;
    }
    else {
        next->prev = prev;
    }

    list->len--;

    IntMapNode_free(node);
}

char *IntMapList_toString(IntMapList *list) {
    DynamicString *dstr = DynamicString_new();
    DynamicString_append(dstr, "[");

    IntMapNode *curr = list->head;
    char buf[128];
    while (curr != NULL) {
        if (curr->next != NULL) {
            sprintf(buf, "%d=%d, ", curr->key, curr->val);
        } else {
            sprintf(buf, "%d=%d", curr->key, curr->val);
        }
        DynamicString_append(dstr, buf);
        curr = curr->next;
    }

    DynamicString_append(dstr, "]");
    char *str = DynamicString_build(dstr);
    DynamicString_free(dstr);
    return str;
}