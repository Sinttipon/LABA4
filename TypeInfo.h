#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*BinaryOperator)(const void *arg1, const void *arg2, void *result);

typedef struct
{
    size_t size;
    BinaryOperator add;
    BinaryOperator multiply;
    BinaryOperator scalar_multiply;
    void (*print)(const void *);
    void *(*create)(void);
    void (*copy)(void *dest, const void *src);
    void (*free)(void *value);
} TypeInfo;

const TypeInfo *get_int_type_info(void);
const TypeInfo *get_double_type_info(void);
