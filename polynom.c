#include "polynom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PolynomErrors.h"

#define GET_COEF_PTR(poly, index) ((char *)(poly)->coeffs + (index) * (poly)->type_info->size)

Polynom *create_poly(const TypeInfo *type_info, size_t degree, PolyErrors *err)
{
    if (err)
        *err = ok;

    if (!type_info)
    {
        if (err)
            *err = null_error;
        return NULL;
    }

    Polynom *poly = (Polynom *)malloc(sizeof(Polynom));

    if (!poly)
    {
        if (err)
            *err = memory_allocation_failed;
        return NULL;
    }

    poly->degree = degree;
    poly->type_info = type_info;
    poly->coeffs = calloc(degree + 1, type_info->size);

    if (!poly->coeffs)
    {
        free(poly);
        if (err)
            *err = memory_allocation_failed;
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

    if (err)
        *err = ok;
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

PolyErrors add_poly(const Polynom *a, const Polynom *b, Polynom *result)
{
    if (!a || !b || !result || !a->type_info || !b->type_info)
        return null_error;
    if (a->type_info != b->type_info || a->type_info != result->type_info)
        return type_error;

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

    if (result->degree != max_deg)
        return degree_error;

    void *zero = a->type_info->create();

    if (!zero)
        return memory_allocation_failed;

    size_t i;
    for (i = 0; i <= max_deg; i++)
    {
        const void *val_a;
        const void *val_b;

        if (i <= a->degree)
        {
            val_a = GET_COEF_PTR(a, i);
        }

        else
        {
            val_a = zero;
        }

        if (i <= b->degree)
        {
            val_b = GET_COEF_PTR(b, i);
        }
        else
        {
            val_b = zero;
        }
        a->type_info->add(GET_COEF_PTR(result, i), val_a, val_b);
    }

    a->type_info->free(zero);
    return ok;
}

PolyErrors scalar_mul_poly(const Polynom *poly, const void *scalar, Polynom *result)
{
    if (!poly || !scalar || !result || !poly->type_info)
        return null_error;
    if (poly->type_info != result->type_info)
        return type_error;
    if (result->degree != poly->degree)
        return degree_error;

    size_t i;
    for (i = 0; i <= poly->degree; i++)
    {
        poly->type_info->scalar_multiply(GET_COEF_PTR(result, i), GET_COEF_PTR(poly, i), scalar);
    }
    return ok;
}

PolyErrors poly_eval(const Polynom *poly, const void *x, void *result)
{
    if (!poly || !x || !result || !poly->type_info)
        return null_error;

    poly->type_info->copy(result, GET_COEF_PTR(poly, poly->degree));

    int i = (int)poly->degree - 1;
    while (i >= 0)
    {
        poly->type_info->multiply(result, x, result);
        poly->type_info->add(result, GET_COEF_PTR(poly, i), result);
        i--;
    }

    return ok;
}

PolyErrors mul_poly(const Polynom *a, const Polynom *b, Polynom *result)
{
    if (!a || !b || !result || !a->type_info || !b->type_info)
        return null_error;
    if (a->type_info != b->type_info || a->type_info != result->type_info)
        return type_error;

    size_t res_deg = a->degree + b->degree;

    if (result->degree != res_deg)
        return degree_error;

    void *temp_prod = a->type_info->create();
    void *zero = a->type_info->create();

    if (!temp_prod || !zero)
    {
        if (temp_prod)
            a->type_info->free(temp_prod);
        if (zero)
            a->type_info->free(zero);
        return memory_allocation_failed;
    }

    size_t k;
    for (k = 0; k <= res_deg; k++)
    {
        a->type_info->copy(GET_COEF_PTR(result, k), zero);
    }

    size_t i, j;
    for (i = 0; i <= a->degree; i++)
    {
        for (j = 0; j <= b->degree; j++)
        {
            a->type_info->multiply(temp_prod, GET_COEF_PTR(a, i), GET_COEF_PTR(b, j));
            a->type_info->add(GET_COEF_PTR(result, i + j), temp_prod, GET_COEF_PTR(result, i + j));
            a->type_info->copy(temp_prod, zero);
        }
    }

    a->type_info->free(temp_prod);
    a->type_info->free(zero);
    return ok;
}

PolyErrors compose_poly(const Polynom *a, const Polynom *b, Polynom *result)
{
    if (!a || !b || !result || !a->type_info || !b->type_info)
        return null_error;
    if (a->type_info != b->type_info || a->type_info != result->type_info)
        return type_error;

    size_t res_deg = a->degree * b->degree;
    if (result->degree != res_deg)
        return degree_error;

    void *zero_val = a->type_info->create();
    if (!zero_val)
        return memory_allocation_failed;

    size_t k;
    for (k = 0; k <= res_deg; k++)
    {
        a->type_info->copy(GET_COEF_PTR(result, k), zero_val);
    }

    if (a->degree == 0)
    {
        a->type_info->copy(GET_COEF_PTR(result, 0), GET_COEF_PTR(a, 0));
        a->type_info->free(zero_val);
        return ok;
    }


    Polynom *current = create_poly(a->type_info, 0, NULL);

    if (!current)
    {
        a->type_info->free(zero_val);
        return memory_allocation_failed;
    }
    poly_set_coef(current, 0, GET_COEF_PTR(a, a->degree));

    int i = (int)a->degree - 1;
    while (i >= 0)
    {
        size_t next_deg = current->degree + b->degree;
        Polynom *next = create_poly(a->type_info, next_deg, NULL);

        if (!next)
        {
            delete_poly(current);
            a->type_info->free(zero_val);
            return memory_allocation_failed;
        }

        PolyErrors err = mul_poly(current, b, next);
        delete_poly(current);
        if (err != ok)
        {
            delete_poly(next);
            a->type_info->free(zero_val);
            return err;
        }
        current = next;

        Polynom *const_poly = create_poly(a->type_info, 0, NULL);
        if (!const_poly)
        {
            delete_poly(current);
            a->type_info->free(zero_val);
            return memory_allocation_failed;
        }
        poly_set_coef(const_poly, 0, GET_COEF_PTR(a, i));

        Polynom *sum = create_poly(a->type_info, current->degree, NULL);
        if (!sum)
        {
            delete_poly(current);
            delete_poly(const_poly);
            a->type_info->free(zero_val);
            return memory_allocation_failed;
        }

        err = add_poly(current, const_poly, sum);
        delete_poly(current);
        delete_poly(const_poly);

        if (err != ok)
        {
            delete_poly(sum);
            a->type_info->free(zero_val);
            return err;
        }
        current = sum;
        i--;
    }

    for (k = 0; k <= result->degree; k++)
    {
        if (k <= current->degree)
            a->type_info->copy(GET_COEF_PTR(result, k), GET_COEF_PTR(current, k));
        else
            a->type_info->copy(GET_COEF_PTR(result, k), zero_val);
    }

    delete_poly(current);
    a->type_info->free(zero_val);

    return ok;
}

void poly_print(const Polynom *poly)
{
    if (!poly)
    {
        printf("NULL многочлен\n");
        return;
    }

    int first_term = 1;
    int has_terms = 0;
    int i = (int)poly->degree;

    while (i >= 0)
    {
        void *coef = GET_COEF_PTR(poly, i);

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