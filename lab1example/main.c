#include "Vector.h"

int main()
{
    int x1 = 1, y1 = 2, z1 = 3;
    int x2 = 4, y2 = 5, z2 = 6;
    int x3 = 0, y3 = 0, z3 = 0;
    VectorErrors operationResult = VECTOR_OPERATION_OK;

    Vector* v1 = createVector(GetIntTypeInfo(), &x1, &y1, &z1, &operationResult);
    Vector* v2 = createVector(GetIntTypeInfo(), &x2, &y2, &z2, &operationResult);
    Vector* sum = createVector(GetIntTypeInfo(), &x3, &y3, &z3, &operationResult);

    printf("Vector 1: ");
    printVector(v1);

    printf("Vector 2: ");
    printVector(v2);

    addVectors(v1, v2, sum);

    printf("Sum of vectors: ");
    printVector(sum);

    freeVector(v1);
    freeVector(v2);
    freeVector(sum);

    return 0;
}
