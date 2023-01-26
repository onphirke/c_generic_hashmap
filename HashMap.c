#include "HashMap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DynamicString.h"

// ------------------------------------------------------
// ------------------------------------------------------
// --------------main hashmap implementation-------------
// ------------------------------------------------------
// ------------------------------------------------------

// ---------------constructor and freers-----------------
// ---------------constructor and freers-----------------
// ---------------constructor and freers-----------------

HashMapNode *HashMapNode_new(void *key, void *val, HashMapNode *next) {
    HashMapNode *node = (HashMapNode *)malloc(sizeof(HashMapNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->val = val;
    node->next = next;
    return node;
}

// this method only deletes the pointers to data
// it does NOT free cloned key, cloned val, or next
void HashMapNode_free(HashMapNode *node, HashMap *map) {
    map->keySpecs.destroy(node->key);
    map->valSpecs.destroy(node->val);
    free(node);
}

HashMap *HashMap_new(HashMapKeySpecifier keySpecs, HashMapValueSpecifier valSpecs) {
    HashMap *map = malloc(sizeof(HashMap));
    if (map == NULL) {
        return NULL;
    }

    map->tableSize = 16;
    map->table = malloc(map->tableSize * sizeof(HashMapNode *));
    if (map->table == NULL) {
        free(map);
        return NULL;
    }

    map->keySpecs = keySpecs;
    map->valSpecs = valSpecs;
    map->loadFactor = 0.9f;
    map->size = 0;

    for (int i = 0; i < map->tableSize; i++) {
        map->table[i] = NULL;
    }
    return map;
}

void HashMap_free(HashMap *map) {
    for (int i = 0; i < map->tableSize; i++) {
        if (map->table[i] == NULL) {
            continue;
        }
        HashMapNode *node = map->table[i];
        while (node != NULL) {
            HashMapNode *next = node->next;
            HashMapNode_free(node, map);
            node = next;
        }
    }
    free(map->table);
    free(map);
}

// ----------------statics------------------
// ----------------statics------------------
// ----------------statics------------------

static int HashMap_computeBucket(HashMap *map, void *key);
static void HashMap_resize(HashMap *map, int newSize);
static void HashMap_maybeResize(HashMap *map);
static HashMapNode *HashMap_findNodeWithKey(HashMap *map, void *key);

static int HashMap_computeBucket(HashMap *map, void *key) {
    return (map->keySpecs.hash(key) % map->tableSize + map->tableSize) % map->tableSize;
}

static void HashMap_resize(HashMap *map, int newSize) {
    // ensure the new tableSize is greater than 8
    if (newSize < 8) {
        newSize = 8;
    }

    // create a list of all the nodes
    HashMapNode *head = NULL;
    HashMapNode *tail = NULL;
    for (int i = 0; i < map->tableSize; i++) {
        if (map->table[i] == NULL) {
            continue;
        }
        if (head == NULL) {
            head = tail = map->table[i];
        } else {
            tail->next = map->table[i];
        }
        map->table[i] = NULL;
        while (tail->next != NULL) {
            tail = tail->next;
        }
    }

    // resize the table
    map->table = realloc(map->table, newSize * sizeof(HashMapNode *));
    for (int i = map->tableSize; i < newSize; i++) {
        map->table[i] = NULL;
    }
    map->tableSize = newSize;

    // reinsert all the nodes
    while (head != NULL) {
        HashMapNode *next = head->next;
        head->next = NULL;

        int bucket = HashMap_computeBucket(map, head->key);
        HashMapNode *node = map->table[bucket];
        if (node == NULL) {
            map->table[bucket] = head;
        } else {
            while (node->next != NULL) {
                node = node->next;
            }
            node->next = head;
        }

        head = next;
    }
}

static void HashMap_maybeResize(HashMap *map) {
    int newSize = map->tableSize << 1;
    if (map->tableSize > newSize) {
        newSize = 2147483647;
    }
    if (map->size > (map->tableSize * map->loadFactor)) {
        HashMap_resize(map, newSize);
    }
}

static HashMapNode *HashMap_findNodeWithKey(HashMap *map, void *key) {
    // first compute bucket
    int bucket = HashMap_computeBucket(map, key);

    // search for the key
    HashMapNode *node = map->table[bucket];
    while (node != NULL) {
        if (map->keySpecs.equals(node->key, key)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// -----------------globals-----------------------
// -----------------globals-----------------------
// -----------------globals-----------------------

void HashMap_put(HashMap *map, void *key, void *val) {
    // first compute bucket
    int bucket = HashMap_computeBucket(map, key);

    // put the pair at the bucket
    HashMapNode *node = map->table[bucket];

    // see if there is a node
    if (node == NULL) {
        void *keyCopy = map->keySpecs.clone(key);
        void *valCopy = map->valSpecs.clone(val);
        map->table[bucket] = HashMapNode_new(keyCopy, valCopy, NULL);
        map->size++;
        HashMap_maybeResize(map);
        return;
    }

    // otherwise, search for the key
    while (node->next != NULL) {
        if (map->keySpecs.equals(node->key, key)) {
            // replace the value
            map->valSpecs.destroy(node->val);
            node->val = map->valSpecs.clone(val);
            return;
        }
        node = node->next;
    }
    if (map->keySpecs.equals(node->key, key)) {
        // replace the value
        map->valSpecs.destroy(node->val);
        node->val = map->valSpecs.clone(val);
        return;
    }

    // didn't find it, so add the node
    void *keyCopy = map->keySpecs.clone(key);
    void *valCopy = map->valSpecs.clone(val);
    node->next = HashMapNode_new(keyCopy, valCopy, NULL);
    map->size++;
    HashMap_maybeResize(map);
}

void *HashMap_get(HashMap *map, void *key) {
    HashMapNode *node = HashMap_findNodeWithKey(map, key);
    if (node == NULL) {
        return NULL;
    }
    return map->valSpecs.clone(node->val);
}

int HashMap_containsKey(HashMap *map, void *key) {
    HashMapNode *node = HashMap_findNodeWithKey(map, key);
    return node != NULL;
}

void HashMap_remove(HashMap *map, void *key) {
    // first compute bucket
    int bucket = HashMap_computeBucket(map, key);

    // search for the key
    HashMapNode *prev = NULL;
    HashMapNode *node = map->table[bucket];
    while (node != NULL) {
        if (map->keySpecs.equals(node->key, key)) {
            if (prev == NULL) {
                map->table[bucket] = node->next;
            } else {
                prev->next = node->next;
            }
            HashMapNode_free(node, map);
            return;
        }
        prev = node;
        node = node->next;
    }
}

char *HashMap_toString(HashMap *map, char *(*keyToString)(void *key), char *(*valToString)(void *val)) {
    DynamicString *dstr = DynamicString_new();

    char buf[16];
    for (int i = 0; i < map->tableSize; i++) {
        sprintf(buf, "%d : ", i);
        DynamicString_append(dstr, buf);

        HashMapNode *node = map->table[i];
        if (node == NULL) {
            DynamicString_append(dstr, "\n");
            continue;
        }

        DynamicString_append(dstr, "[");
        while (node != NULL) {
            char *keyString = keyToString(node->key);
            DynamicString_append(dstr, keyString);
            free(keyString);

            DynamicString_append(dstr, "=");

            char *valString = valToString(node->val);
            DynamicString_append(dstr, valString);
            free(valString);

            if (node->next != NULL) {
                DynamicString_append(dstr, ", ");
            }

            node = node->next;
        }
        DynamicString_append(dstr, "]");
        DynamicString_append(dstr, "\n");
    }

    char *str = DynamicString_build(dstr);
    DynamicString_free(dstr);
    return str;
}

// -----------------default templates-------------------
// -----------------default templates-------------------
// -----------------default templates-------------------

// -----------------integer template--------------------
static int HashMap_template_INTEGER_hash(void *obj) {
    return (int)(long long)(obj);
}

static int HashMap_template_INTEGER_equals(void *obj1, void *obj2) {
    return ((int)(long long)obj1) == ((int)(long long)obj2);
}

static void *HashMap_template_INTEGER_clone(void *obj) {
    return obj;
}

static void HashMap_template_INTEGER_destroy(void *obj) {
}

char *HashMap_template_INTEGER_toString(void *obj) {
    char *buf = malloc(16 * sizeof(char));
    sprintf(buf, "%d", (int)(long)obj);
    return buf;
}

HashMapKeySpecifier INTEGER_HashMapKeySpecifier = {
    HashMap_template_INTEGER_hash,
    HashMap_template_INTEGER_equals,
    HashMap_template_INTEGER_clone,
    HashMap_template_INTEGER_destroy,
};

HashMapValueSpecifier INTEGER_HashMapValueSpecifier = {
    HashMap_template_INTEGER_clone,
    HashMap_template_INTEGER_destroy,
};

// ---------------string template-------------------

static int HashMap_template_STRING_hash(void *obj) {
    int hash = 769;
    char *str = (char *)obj;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash << 5) + hash + str[i];
        #ifdef DEBUG_HASHMAP
        printf("%d\n", hash);
        if (str[i+1] == '\0') {
            printf("done");
        }
        #endif
    }
    return hash;
}

static int HashMap_template_STRING_equals(void *obj1, void *obj2) {
    return strcmp((char *)obj1, (char *)obj2) == 0;
}

static void *HashMap_template_STRING_clone(void *obj) {
    return strdup((char *)obj);
}

static void HashMap_template_STRING_destroy(void *obj) {
    free(obj);
}

char *HashMap_template_STRING_toString(void *obj) {
    return strdup((char *)obj);
}

HashMapKeySpecifier STRING_HashMapKeySpecifier = {
    HashMap_template_STRING_hash,
    HashMap_template_STRING_equals,
    HashMap_template_STRING_clone,
    HashMap_template_STRING_destroy,
};

HashMapValueSpecifier STRING_HashMapValueSpecifier = {
    HashMap_template_STRING_clone,
    HashMap_template_STRING_destroy,
};