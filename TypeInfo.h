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
} TypeInfo;
