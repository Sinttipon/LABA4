#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "polynom.h"

int is_number(int c);

int get_degree(const char *str, int **degrees, int *count);

int get_max_degree(const char *str);

void sort_degrees(int *degrees, int count);

Polynom *parse_polynomial(const char *str, const TypeInfo *type, PolyErrors *err);