#ifndef DOUBLE_H
#define DOUBLE_H

#include "TypeInfo.h"

void doubleAdd(const void* arg1, const void* arg2, void* result);
void doubleMultiply(const void* arg1, const void* arg2, void* result);
void doublePrint(const void* data);
TypeInfo* GetDoubleTypeInfo();

#endif // DOUBLE_H
