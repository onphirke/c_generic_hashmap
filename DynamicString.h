typedef struct DynamicString {
    char *data;
    int bufSize;
    unsigned int len;
} DynamicString;

extern DynamicString *DynamicString_new(void);
extern void DynamicString_free(DynamicString *dstr);
extern void DynamicString_append(DynamicString *dstr, char *str);
extern void DynamicString_resize(DynamicString *dstr, int newBufSize);
extern void DynamicString_trimBuffer(DynamicString *dstr);
extern char *DynamicString_build(DynamicString *dstr);