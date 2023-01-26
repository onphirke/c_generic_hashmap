#include "IntMapList.h"

// map
typedef struct HashIntMap {
    IntMapList **table;
    int tableSize;
    float loadFactor;
    int size;
} HashIntMap;

extern HashIntMap *HashIntMap_new(float loadFactor);
extern int *HashIntMap_put(HashIntMap *map, int key, int val);
extern int *HashIntMap_get(HashIntMap *map, int key);
extern int *HashIntMap_containsKey(HashIntMap *map, int key);
extern int *HashIntMap_size(HashIntMap *map);
extern int *HashIntMap_resize(HashIntMap *map, int newSize);


