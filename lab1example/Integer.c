#include "integer.h"

static TypeInfo* INT_TYPE_INFO = NULL;


void intAdd(const void* arg1, const void* arg2, void* result) {
    *(int*)result = *(int*)arg1 + *(int*)arg2;
}

void intMultiply(const void* arg1, const void* arg2, void* result) {
    *(int*)result = *(int*)arg1 * *(int*)arg2;
}

void intPrint(const void* data) {
    printf("%d", *(const int*)data);
}

TypeInfo* GetIntTypeInfo() {
    if (INT_TYPE_INFO == NULL) {
        INT_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        INT_TYPE_INFO->size = sizeof(int);
        INT_TYPE_INFO->add = intAdd;
        INT_TYPE_INFO->multiply = intMultiply;
        INT_TYPE_INFO->print = intPrint;
    }
    return INT_TYPE_INFO;
}