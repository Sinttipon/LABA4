#pragma once
#include "TypeInfo.h"

const TypeInfo *get_int_type_info(void);

void* int_create(void);

void int_copy(void *dest, const void *src);

void int_add(void *result, const void *first, const void *second);

void int_multi(void *result, const void *first, const void *second);

void int_scalar_multi(void *result, const void *first, const void *second);

void int_print(const void *value);

void int_free(void *value);
