#include "polynom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PolynomErrors.h"

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
    poly->coef = (void **)calloc(degree + 1, sizeof(void *));

    if (!poly->coef)
    {
        free(poly);
        return create_poly_error;
    }

    for (size_t i = 0; i <= degree; i++)
    {
        poly->coef[i] = create_zero(type_info);
        if (!poly->coef[i])
        {
            for (size_t k = 0; k < i; k++)
            {
                type_info->free(poly->coef[k]);
            }
            free(poly->coef);
            free(poly);
            return create_poly_error;
        }
    }
    return poly;
}

void delete_poly(Polynom *poly)
{
    if (!poly)
        return;

    if (poly->coef && poly->type_info)
    {
        for (size_t i = 0; i <= poly->degree; i++)
        {
            if (poly->coef[i])
            {
                poly->type_info->free(poly->coef[i]);
            }
        }
        free(poly->coef);
    }
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
        return get_coef_error;
    return poly->coef[index];
}

void poly_set_coef(Polynom *poly, size_t index, const void *value)
{
    if (!poly || !value || index > poly->degree)
        return;
    poly->type_info->copy(poly->coef[index], value);
}

Polynom *add_poly(const Polynom *a, const Polynom *b)
{
    if (!a || !b || !a->type_info || !b->type_info)
        return null_error;

    if (a->type_info != b->type_info)
    {
        return type_error;
    }

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
        return create_poly_error;

    for (size_t i = 0; i <= max_deg; i++)
    {
        const void *val_a = NULL;
        const void *val_b = NULL;
        void *zero_a = NULL;
        void *zero_b = NULL;

        if (i <= a->degree)
        {
            val_a = a->coef[i];
        }
        else
        {
            zero_a = create_zero(a->type_info);
            val_a = zero_a;
        }

        if (i <= b->degree)
        {
            val_b = b->coef[i];
        }
        else
        {
            zero_b = create_zero(a->type_info);
            val_b = zero_b;
        }

        a->type_info->add(val_a, val_b, res->coef[i]);

        if (zero_a)
        {
            a->type_info->free(zero_a);
        }
        if (zero_b)
        {
            a->type_info->free(zero_b);
        }
    }

    return res;
}

Polynom *scalar_mul_poly(const Polynom *poly, const void *scalar)
{
    if (!poly || !scalar || !poly->type_info)
        return null_error;

    Polynom *res = create_poly(poly->type_info, poly->degree);
    if (!res)
        return create_poly_error;

    for (size_t i = 0; i <= poly->degree; i++)
    {
        poly->type_info->scalar_multiply(poly->coef[i], scalar, res->coef[i]);
    }
    return res;
}

void *poly_eval(const Polynom *poly, const void *x)
{
    if (!poly || !x || !poly->type_info)
        return null_error;

    void *result = poly->type_info->create();
    if (!result)
        return create_poly_error;

    poly->type_info->copy(result, poly->coef[poly->degree]);

    int i = (int)poly->degree - 1;
    while (i >= 0)
    {
        poly->type_info->multiply(result, x, result);
        poly->type_info->add(result, poly->coef[i], result);

        i--;
    }

    return result;
}

Polynom *mul_poly(const Polynom *a, const Polynom *b)
{
    if (!a || !b || !a->type_info || !b->type_info)
        return null_error;
    if (a->type_info != b->type_info)
        return type_error;

    size_t res_deg = a->degree + b->degree;
    Polynom *res = create_poly(a->type_info, res_deg);
    if (!res)
        return create_poly_error;

    void *temp_prod = a->type_info->create();
    void *zero = create_zero(a->type_info);

    if (!temp_prod || !zero)
    {
        if (temp_prod)
            a->type_info->free(temp_prod);
        if (zero)
            a->type_info->free(zero);
        delete_poly(res);
        return NULL;
    }

    for (size_t i = 0; i <= a->degree; i++)
    {
        for (size_t j = 0; j <= b->degree; j++)
        {
            a->type_info->multiply(a->coef[i], b->coef[j], temp_prod);
            a->type_info->add(res->coef[i + j], temp_prod, res->coef[i + j]);
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
        return null_error;
    if (p->type_info != q->type_info)
        return type_error;

    size_t res_deg = p->degree * q->degree;
    Polynom *res = create_poly(p->type_info, res_deg);
    if (!res)
        return create_poly_error;

    Polynom *current = create_poly(p->type_info, 0);
    if (!current)
    {
        delete_poly(res);
        return create_poly_error;
    }
    poly_set_coef(current, 0, p->coef[p->degree]);

    void *zero_coef = create_zero(p->type_info);

    int i = (int)p->degree - 1;
    while (i >= 0)
    {
        Polynom *next = mul_poly(current, q);
        delete_poly(current);
        if (!next)
        {
            p->type_info->free(zero_coef);
            delete_poly(res);
            return operation_failed;
        }
        current = next;

        Polynom *const_poly = create_poly(p->type_info, 0);
        if (!const_poly)
        {
            delete_poly(current);
            p->type_info->free(zero_coef);
            delete_poly(res);
            return operation_failed;
        }
        poly_set_coef(const_poly, 0, p->coef[i]);

        Polynom *sum = add_poly(current, const_poly);
        delete_poly(current);
        delete_poly(const_poly);

        if (!sum)
        {
            p->type_info->free(zero_coef);
            delete_poly(res);
            return operation_failed;
        }
        current = sum;

        i--;
    }

    for (size_t k = 0; k <= res->degree; k++)
    {
        if (k <= current->degree)
        {
            p->type_info->copy(res->coef[k], current->coef[k]);
        }
        else
        {
            p->type_info->copy(res->coef[k], zero_coef);
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

    if (poly->degree == 0)
    {
        if (poly->coef[0])
        {
            poly->type_info->print(poly->coef[0]);
        }
        else
        {
            printf("0");
        }
        printf("\n");
        return;
    }

    int first_term = 1;
    int has_terms = 0;

    int i = (int)poly->degree;
    while (i >= 0)
    {
        void *coef = poly->coef[i];
        if (!coef)
        {
            i--;
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