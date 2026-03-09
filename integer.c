#include "integer.h"
#include <stdlib.h>
#include <stdio.h>

void *int_create(void)
{
    int *val = (int *)malloc(sizeof(int));
    if (val)
    {
        *val = 0;
    }
    return val;
}

void int_copy(void *dest, const void *src)
{
    if (dest && src)
    {
        *(int*)dest = *(const int*)src;
    }
}

void int_add(void *result, const void *first, const void *second)
{
    if (result && first && second)
    {
        *(int*)result = *(const int*)first + *(const int*)second;
    }
}

void int_multi(void *result, const void *first, const void *second)
{
    if (result && first && second)
    {
        *(int *)result = *(const int *)first * *(const int *)second;
    }
}

void int_scalar_multi(void *result, const void *first, const void *second)
{
    int_multi(result, first, second);
}

void int_print(const void *value)
{
    if (value)
    {
        printf("%d", *(const int *)value);
    }
}

void int_free(void *value)
{
    free(value);
}