// node object
typedef struct IntMapNode {
    struct IntMapNode *prev;
    struct IntMapNode *next;
    int key;
    int val;
} IntMapNode;

extern IntMapNode *IntMapNode_new(int key, int val, IntMapNode *prev, IntMapNode *next);
extern void IntMapNode_free(IntMapNode *node);

// list for map
typedef struct IntMapList {
    IntMapNode *head;
    IntMapNode *tail;
    int len;
} IntMapList;

extern IntMapList *IntMapList_new(void);
extern void IntMapList_free(IntMapList *list);
extern IntMapNode *IntMapList_put(IntMapList *list, int key, int val);
extern IntMapNode *IntMapList_find(IntMapList *list, int key);
extern void IntMapList_remove(IntMapList *list, int key);
extern char *IntMapList_toString(IntMapList *list);

