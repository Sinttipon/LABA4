#pragma once
#include "TypeInfo.h"
typedef struct
{
    void *coef;          
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


