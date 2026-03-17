#include "polynom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PolynomErrors.h"

#define GET_COEF_PTR(poly, index) ((char *)(poly)->coeffs + (index) * (poly)->type_info->size)

static void *create_zero(const TypeInfo *info)
{
    return info->create();
}

Polynom *create_poly(const TypeInfo *type_info, size_t degree)
{
    if (!type_info)
        return create_poly_error;

    Polynom *poly = (Polynom *)malloc(sizeof(Polynom));

    if (!poly)
        return memory_allocation_failed;

    poly->degree = degree;
    poly->type_info = type_info;
    poly->coeffs = calloc(degree + 1, type_info->size);
    if (!poly->coeffs)
    {
        free(poly);
        return NULL;
    }

    void *zero = type_info->create();
    if (zero)
    {
        size_t i;
        for (i = 0; i <= degree; i++)
        {
            type_info->copy(GET_COEF_PTR(poly, i), zero);
        }
        type_info->free(zero);
    }
    return poly;
}

void delete_poly(Polynom *poly)
{
    if (!poly)
        return;
    free(poly->coeffs);
    free(poly);
}

size_t poly_get_degree(const Polynom *poly)
{
    if (!poly)
        return 0;
    return poly->degree;
}

const void *poly_get_coef(const Polynom *poly, size_t index)
{
    if (!poly || index > poly->degree)
        return NULL;
    return GET_COEF_PTR(poly, index);
}

void poly_set_coef(Polynom *poly, size_t index, const void *value)
{
    if (!poly || !value || index > poly->degree)
        return;
    poly->type_info->copy(GET_COEF_PTR(poly, index), value);
}

Polynom *add_poly(const Polynom *a, const Polynom *b)
{
    if (!a || !b || !a->type_info || !b->type_info)
        return NULL;
    if (a->type_info != b->type_info)
        return NULL;

    size_t deg_a = a->degree;
    size_t deg_b = b->degree;
    size_t max_deg;
    if (deg_a > deg_b)
    {
        max_deg = deg_a;
    }
    else
    {
        max_deg = deg_b;
    }

    Polynom *res = create_poly(a->type_info, max_deg);
    if (!res)
        return NULL;

    void *zero = a->type_info->create();
    size_t i;
    for (i = 0; i <= max_deg; i++)
    {
        const void *val_a = zero;
        const void *val_b = zero;
        if (i <= a->degree)
        {
            val_a = GET_COEF_PTR(a, i);
        }

        if (i <= b->degree)
        {
            val_b = GET_COEF_PTR(b, i);
        }

        a->type_info->add(val_a, val_b, GET_COEF_PTR(res, i));
    }
    a->type_info->free(zero);

    return res;
}

Polynom *scalar_mul_poly(const Polynom *poly, const void *scalar)
{
    if (!poly || !scalar || !poly->type_info)
        return NULL;

    Polynom *res = create_poly(poly->type_info, poly->degree);
    if (!res)
        return NULL;

    size_t i;
    for (i = 0; i <= poly->degree; i++)
    {
        poly->type_info->scalar_multiply(GET_COEF_PTR(poly, i), scalar, GET_COEF_PTR(res, i));
    }
    return res;
}

void *poly_eval(const Polynom *poly, const void *x)
{
    if (!poly || !x || !poly->type_info)
        return NULL;

    void *result = poly->type_info->create();
    if (!result)
        return NULL;

    poly->type_info->copy(result, GET_COEF_PTR(poly, poly->degree));

    int i = (int)poly->degree - 1;
    while (i >= 0)
    {
        poly->type_info->multiply(result, x, result);
        poly->type_info->add(result, GET_COEF_PTR(poly, i), result);
        i--;
    }

    return result;
}

Polynom *mul_poly(const Polynom *a, const Polynom *b)
{
    if (!a || !b || !a->type_info || !b->type_info)
        return NULL;

    if (a->type_info != b->type_info)
        return NULL;

    size_t res_deg = a->degree + b->degree;
    Polynom *res = create_poly(a->type_info, res_deg);

    if (!res)
        return NULL;

    void *temp_prod = a->type_info->create();
    void *zero = a->type_info->create();

    if (!temp_prod || !zero)
    {
        if (temp_prod)
            a->type_info->free(temp_prod);
        if (zero)
            a->type_info->free(zero);
        delete_poly(res);
        return NULL;
    }

    size_t i, j;
    for (i = 0; i <= a->degree; i++)
    {
        for (j = 0; j <= b->degree; j++)
        {
            a->type_info->multiply(GET_COEF_PTR(a, i), GET_COEF_PTR(b, j), temp_prod);
            a->type_info->add(GET_COEF_PTR(res, i + j), temp_prod, GET_COEF_PTR(res, i + j));
            a->type_info->copy(temp_prod, zero);
        }
    }

    a->type_info->free(temp_prod);
    a->type_info->free(zero);

    return res;
}

Polynom *compose_poly(const Polynom *p, const Polynom *q)
{
    if (!p || !q || !p->type_info || !q->type_info)
        return NULL;
    if (p->type_info != q->type_info)
        return NULL;

    size_t res_deg = p->degree * q->degree;
    Polynom *res = create_poly(p->type_info, res_deg);

    if (!res)
        return NULL;

    if (p->degree == 0)
    {
        p->type_info->copy(GET_COEF_PTR(res, 0), GET_COEF_PTR(p, 0));
        return res;
    }

    Polynom *current = create_poly(p->type_info, 0);
    if (!current)
    {
        delete_poly(res);
        return NULL;
    }
    poly_set_coef(current, 0, GET_COEF_PTR(p, p->degree));

    void *zero_coef = p->type_info->create();
    int i = (int)p->degree - 1;
    while (i >= 0)
    {
        Polynom *next = mul_poly(current, q);
        delete_poly(current);

        if (!next)
        {
            p->type_info->free(zero_coef);
            delete_poly(res);
            return NULL;
        }
        current = next;

        Polynom *const_poly = create_poly(p->type_info, 0);
        if (!const_poly)
        {
            delete_poly(current);
            p->type_info->free(zero_coef);
            delete_poly(res);
            return NULL;
        }
        poly_set_coef(const_poly, 0, GET_COEF_PTR(p, i));

        Polynom *sum = add_poly(current, const_poly);
        delete_poly(current);
        delete_poly(const_poly);

        if (!sum)
        {
            p->type_info->free(zero_coef);
            delete_poly(res);
            return NULL;
        }
        current = sum;
        i--;
    }

    size_t k;
    for (k = 0; k <= res->degree; k++)
    {
        if (k <= current->degree)
        {
            p->type_info->copy(GET_COEF_PTR(res, k), GET_COEF_PTR(current, k));
        }
        else
        {
            p->type_info->copy(GET_COEF_PTR(res, k), zero_coef);
        }
    }

    delete_poly(current);
    p->type_info->free(zero_coef);

    return res;
}

void poly_print(const Polynom *poly)
{
    if (!poly)
    {
        printf("NULL polynomial\n");
        return;
    }

    int first_term = 1;
    int has_terms = 0;
    int i = (int)poly->degree;

    while (i >= 0)
    {
        void *coef = GET_COEF_PTR(poly, i);

        // Проверка на ноль
        int is_zero = 0;
        if (poly->type_info->size == sizeof(int))
        {
            if (*(int *)coef == 0)
                is_zero = 1;
        }
        else if (poly->type_info->size == sizeof(double))
        {
            if (*(double *)coef == 0.0)
                is_zero = 1;
        }

        if (is_zero)
        {
            i--;
            continue;
        }

        if (!first_term)
        {
            printf(" + ");
        }

        poly->type_info->print(coef);

        if (i > 0)
        {
            printf("x");
            if (i > 1)
            {
                printf("^%d", i);
            }
        }

        first_term = 0;
        has_terms = 1;
        i--;
    }

    if (!has_terms)
    {
        printf("0");
    }
    printf("\n");
}