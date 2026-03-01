#ifndef VECTOR_H
#define VECTOR_H

#include "TypeInfo.h"
#include "Integer.h"
#include "Double.h"
#include "VectorErrors.h"

typedef struct {
    void* x;
    void* y;
    void* z;
    TypeInfo* typeInfo;
} Vector;

Vector* createVector(TypeInfo* typeInfo, void* x, void* y, void* z, VectorErrors* operationResult);
void freeVector(Vector* vector);
VectorErrors addVectors(const Vector* v1, const Vector* v2, Vector* result);
VectorErrors multiplyVectors(const Vector* v1, const Vector* v2, Vector* result);
VectorErrors printVector(const Vector* vector);

#endif // VECTOR_H
