#include "double.h"
#include <stdio.h>

static TypeInfo *DBL_TYPE_INFO = NULL;

static void std_clear()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void *dbl_create(void)
{
    double *val = (double *)malloc(sizeof(double));
    if (val)
    {
        *val = 0.0;
    }
    return val;
}

void dbl_copy(void *dest, const void *src)
{
    if (dest && src)
    {
        *(double *)dest = *(const double *)src;
    }
}

void dbl_add(void *result, const void *first, const void *second)
{
    if (result && first && second)
    {
        *(double *)result = *(const double *)first + *(const double *)second;
    }
}

void dbl_sub(void *result, const void *first, const void *second)
{
    if (result && first && second)
    {
        *(double *)result = *(const double *)first - *(const double *)second;
    }
}

void dbl_multi(void *result, const void *first, const void *second)
{
    if (result && first && second)
    {
        *(double *)result = *(const double *)first * *(const double *)second;
    }
}

void dbl_scalar_multi(void *result, const void *first, const void *second)
{
    dbl_multi(result, first, second);
}

static void dbl_scan(void *value)
{
    if (!value)
        return;
    while (1)
    {
        if (scanf("%lf", (double *)value) == 1)
        {
            std_clear();
            return;
        }
        printf("Ошибка ввода вещественного числа. Повторите: ");
        std_clear();
    }
}

void dbl_print(const void *value)
{
    if (value)
    {
        printf("%.6f", *(const double *)value);
    }
}

void dbl_free(void *value)
{
    free(value);
}

const TypeInfo *get_double_type_info(void)
{
    if (DBL_TYPE_INFO == NULL)
    {
        DBL_TYPE_INFO = (TypeInfo *)malloc(sizeof(TypeInfo));
        if (DBL_TYPE_INFO)
        {
            DBL_TYPE_INFO->size = sizeof(double);
            DBL_TYPE_INFO->add = dbl_add;
            DBL_TYPE_INFO->sub = dbl_sub;
            DBL_TYPE_INFO->multiply = dbl_multi;
            DBL_TYPE_INFO->scalar_multiply = dbl_scalar_multi;
            DBL_TYPE_INFO->print = dbl_print;
            DBL_TYPE_INFO->scan = dbl_scan;
            DBL_TYPE_INFO->create = dbl_create;
            DBL_TYPE_INFO->copy = dbl_copy;
            DBL_TYPE_INFO->free = dbl_free;
        }
    }
    return DBL_TYPE_INFO;
}