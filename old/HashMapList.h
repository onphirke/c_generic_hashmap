// #include "HashMap.h"

// // node object
// typedef struct HashMapNode {
//     struct HashMapNode *prev;
//     struct HashMapNode *next;
//     void *key;
//     void *val;
// } HashMapNode;

// extern HashMapNode *HashMapNode_new(void *key, void *val, HashMapNode *prev, HashMapNode *next);
// extern void HashMapNode_free(HashMapNode *node);

// // list for map
// typedef struct HashMapList {
//     HashMapNode *head;
//     HashMapNode *tail;
//     int len;
// } HashMapList;

// extern HashMapList *HashMapList_new(HashMap *);
// extern void HashMapList_free(HashMapList *list);
// extern HashMapNode *HashMapList_put(HashMapList *list, void *key, void *val);
// extern HashMapNode *HashMapList_find(HashMapList *list, void *key);
// extern void HashMapList_remove(HashMapList *list, void *key);
// extern char *HashMapList_toString(HashMapList *list);

