#include "integer.h"
#include <stdio.h>

static TypeInfo *INT_TYPE_INFO=NULL;

static void std_clear()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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

void int_sub(void *result, const void *first, const void *second)
{
    if (result && first && second)
    {
        *(int *)result = *(const int *)first - *(const int *)second;
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
void int_scan(void *value)
{
    if (!value)
        return;
    while (1)
    {
        if (scanf("%d", (int *)value) == 1)
        {
            std_clear();
            return;
        }
        printf("Ошибка ввода числа. Повторите: ");
        std_clear();
    }
}

void int_free(void *value)
{
    free(value);
}

const TypeInfo *get_int_type_info(void)
{
    if (INT_TYPE_INFO == NULL)
    {
        INT_TYPE_INFO = (TypeInfo *)malloc(sizeof(TypeInfo));
        if (INT_TYPE_INFO)
        {
            INT_TYPE_INFO->size = sizeof(int); 
            INT_TYPE_INFO->add = int_add;
            INT_TYPE_INFO->multiply = int_multi;
            INT_TYPE_INFO->scalar_multiply = int_scalar_multi;
            INT_TYPE_INFO->print = int_print;
            INT_TYPE_INFO->scan = int_scan; 
            INT_TYPE_INFO->create = int_create;
            INT_TYPE_INFO->copy = int_copy;
            INT_TYPE_INFO->free = int_free;
        }
    }
    return INT_TYPE_INFO;
}