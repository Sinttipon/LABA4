#pragma once
#include "TypeInfo.h"
#include "PolynomErrors.h"
typedef struct
{
    void *coeffs;
    size_t degree;
    const TypeInfo *type_info;
} Polynom;

Polynom *create_poly(const TypeInfo *type_info, size_t degree, PolyErrors *err);
void delete_poly(Polynom *poly);

PolyErrors add_poly(const Polynom *a, const Polynom *b, Polynom *result);
PolyErrors mul_poly(const Polynom *a, const Polynom *b, Polynom *result);
PolyErrors scalar_mul_poly(const Polynom *poly, const void *scalar, Polynom *result);
PolyErrors compose_poly(const Polynom *p, const Polynom *q, Polynom *result);

PolyErrors poly_eval(const Polynom *poly, const void *x, void *result);

size_t poly_get_degree(const Polynom *poly);
const void *poly_get_coef(const Polynom *poly, size_t index);
void poly_set_coef(Polynom *poly, size_t index, const void *value);

void poly_print(const Polynom *poly);
char *poly_to_string(const Polynom *poly); 