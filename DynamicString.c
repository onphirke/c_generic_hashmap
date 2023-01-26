#include "DynamicString.h"
#include <stdlib.h>
#include <string.h>

DynamicString *DynamicString_new(void) {
    DynamicString *dstr = malloc(sizeof(DynamicString));
    if (dstr != NULL) {
        dstr->bufSize = 16;
        dstr->len = 0;
        dstr->data = malloc(sizeof(char) * dstr->bufSize);
        if (dstr->data != NULL) {
            dstr->data[0] = '\n';
        }
    }
    return dstr;
}

void DynamicString_free(DynamicString *dstr) {
    free(dstr->data);
    free(dstr);
}

void DynamicString_append(DynamicString *dstr, char *str) {
    int di = dstr->len;
    int si = 0;
    for (;;) {
        if(di == dstr->bufSize) {
            DynamicString_resize(dstr, dstr->bufSize << 1);
        }
        dstr->data[di] = str[si];
        if (str[si] == '\0') {
            break;
        }
        di++;
        si++;
    }
    dstr->len = di;
}

void DynamicString_resize(DynamicString *dstr, int newBufSize) {
    int finalNewSize = dstr->len + 1;
    if (newBufSize > finalNewSize) {
        finalNewSize = newBufSize;
    }
    dstr->data = realloc(dstr->data, newBufSize);
    dstr->bufSize = newBufSize;
}

void DynamicString_trimBuffer(DynamicString *dstr) {
    DynamicString_resize(dstr, dstr->len + 1);
}

char *DynamicString_build(DynamicString *dstr) {
    char *str = malloc(sizeof(char) * dstr->len + 1);
    strcpy(str, dstr->data);
    return str;
}