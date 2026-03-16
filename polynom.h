#pragma once
#include "TypeInfo.h"
typedef struct
{
    void** coef;          
    size_t degree;            
    const TypeInfo *type_info; 
} Polynom;


Polynom *create_poly(const TypeInfo *type_info, size_t degree);
void delete_poly(Polynom *poly);
void poly_print(const Polynom *poly);

Polynom *add_poly(const Polynom *a, const Polynom *b);
Polynom *mul_poly(const Polynom *a, const Polynom *b);
Polynom *scalar_mul_poly(const Polynom *poly, const void *scalar);
Polynom *compose_poly(const Polynom *out, const Polynom *in);

void *poly_eval(const Polynom *poly, const void *x);
size_t poly_get_degree(const Polynom *poly);
const void *poly_get_coef(const Polynom *poly, size_t index);
void poly_set_coef(Polynom *poly, size_t index, const void *value);