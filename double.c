#include "double.h"
#include "TypeInfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

static TypeInfo double_type_info = {
    .size = sizeof(double),
    .add = dbl_add,
    .multiply = dbl_multi,
    .scalar_multiply = dbl_scalar_multi,
    .print = dbl_print,
    .create = dbl_create,
    .copy = dbl_copy,
    .free = dbl_free};

const TypeInfo *get_double_type_info(void)
{
    return &double_type_info;
}