#include "Vector.h"



Vector* createVector(TypeInfo* typeInfo, void* x, void* y, void* z, VectorErrors* operationResult) {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (vector == NULL) {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    vector->typeInfo = typeInfo;
    vector->x = malloc(typeInfo->size);
    vector->y = malloc(typeInfo->size);
    vector->z = malloc(typeInfo->size);
    memcpy(vector->x, x, typeInfo->size);
    memcpy(vector->y, y, typeInfo->size);
    memcpy(vector->z, z, typeInfo->size);

    *operationResult = VECTOR_OPERATION_OK;
    return vector;
}

void freeVector(Vector* vector) {
    free(vector->x);
    free(vector->y);
    free(vector->z);
    free(vector);
}

VectorErrors addVectors(const Vector* v1, const Vector* v2, Vector* result) {
    if (v1 == NULL || v2 == NULL || result == NULL) return VECTOR_NOT_DEFINED;
    if (v1->typeInfo != v2->typeInfo || v1->typeInfo != result->typeInfo) return INCOMPATIBLE_VECTOR_TYPES;
    if (v1->typeInfo->add == NULL) return OPERATION_NOT_DEFINED;

    v1->typeInfo->add(v1->x, v2->x, result->x);
    v1->typeInfo->add(v1->y, v2->y, result->y);
    v1->typeInfo->add(v1->z, v2->z, result->z);

    return VECTOR_OPERATION_OK;
}

VectorErrors multiplyVectors(const Vector* v1, const Vector* v2, Vector* result) {
    //аналогично
    return VECTOR_OPERATION_OK;
}

VectorErrors printVector(const Vector* vector) {
    if (vector == NULL) return VECTOR_NOT_DEFINED;
    if (vector->typeInfo->print == NULL) return OPERATION_NOT_DEFINED;

    printf("Vector: (");
    vector->typeInfo->print(vector->x);
    printf(", ");
    vector->typeInfo->print(vector->y);
    printf(", ");
    vector->typeInfo->print(vector->z);
    printf(")\n");

    return VECTOR_OPERATION_OK;
}