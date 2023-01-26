#ifndef _HASHMAP_H
#define _HASHMAP_H 1

#ifdef DEBUG
#define DEBUG_HASHMAP
#endif

typedef struct HashMapKeySpecifier HashMapKeySpecifier;
typedef struct HashMapValueSpecifier HashMapValueSpecifier;
typedef struct HashMapNode HashMapNode;
typedef struct HashMap HashMap;

// the critical needs for the hashmap keys
// this map creates clones of the keys
struct HashMapKeySpecifier {
    // digests the objects into a hash
    int (*hash)(void *obj);

    // compares two objects and checks that they are equal
    int (*equals)(void *obj1, void *obj2);

    // clones an object and returns its memory address
    void *(*clone)(void *obj);

    // destroys an object, free it from memory
    void (*destroy)(void *obj);
};

// the critical needs for the hashmap values
// this map creates clones of the values
struct HashMapValueSpecifier {
    // clones an object and returns its memory address
    void *(*clone)(void *obj);

    // destroys an object, free it from memory
    void (*destroy)(void *obj);
};

// node object
struct HashMapNode {
    struct HashMapNode *next;
    void *key;
    void *val;
};

extern HashMapNode *HashMapNode_new(void *key, void *val, HashMapNode *next);
extern void HashMapNode_free(HashMapNode *node, HashMap *map);

/**
 * create a map by specifying the key specifier and value specifier to a map
 * this map clones the given keys and values
 * this map returns cloned versions of values
 * this is in order to prevent memory leaks and maintain encapsulation
 * larger structures can be easily used by using pointers as keys and values
 */
struct HashMap {
    HashMapKeySpecifier keySpecs;
    HashMapValueSpecifier valSpecs;
    HashMapNode **table;
    int tableSize;
    float loadFactor;
    int size;
};

extern HashMap *HashMap_new(HashMapKeySpecifier keySpecs, HashMapValueSpecifier valSpecs);
extern void HashMap_free(HashMap *map);

extern void HashMap_put(HashMap *map, void *key, void *val);
extern void *HashMap_get(HashMap *map, void *key);
extern void HashMap_remove(HashMap *map, void *key);
extern int HashMap_containsKey(HashMap *map, void *key);
extern char *HashMap_toString(HashMap *map, char *(*keyToString)(void *key), char *(*valToString)(void *val));

// hash map templates

extern HashMapKeySpecifier INTEGER_HashMapKeySpecifier;
extern HashMapValueSpecifier INTEGER_HashMapValueSpecifier;
extern char *HashMap_template_INTEGER_toString(void *obj);

extern HashMapKeySpecifier STRING_HashMapKeySpecifier;
extern HashMapValueSpecifier STRING_HashMapValueSpecifier;
extern char *HashMap_template_STRING_toString(void *obj);

#endif  // !_HASHMAP_H