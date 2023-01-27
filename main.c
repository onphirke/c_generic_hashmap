

// #include "HashMap.h"
#include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

int main(int argc, char *argv[]) {
    // integer testing
    // HashMap *map = HashMap_new(INTEGER_HashMapKeySpecifier, INTEGER_HashMapValueSpecifier);
    // for(long long i = 0; i < 100; i++) {
    //     HashMap_put(map, (void *) i, (void *) -i);
    // }
    // char *mapString = HashMap_toString(map, HashMap_template_INTEGER_toString, HashMap_template_INTEGER_toString);
    // printf("%s", mapString);
    // printf("\n\n");



    HashMap *map = HashMap_new(STRING_HashMapKeySpecifier, STRING_HashMapValueSpecifier);
    for(int i = 0; i < argc; i++) {
        HashMap_put(map, argv[i], argv[i]);
    }

    HashMap_put(map, "wowowowhurieughriihgrueih", "hohohoh");

    char *mapString = HashMap_toString(map, HashMap_template_STRING_toString, HashMap_template_STRING_toString);
    printf("%s", mapString);
    
    free(mapString);
    HashMap_free(map);

    return 0;
}