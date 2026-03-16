#pragma once 

typedef enum{
    poli_parse_errore=100,
    invalid_input=200,
    null_error=300,
    create_poly_error=400,
    get_coef_error=500,
    type_error=600,
    operation_failed=700,
    ok = 0,
    memory_allocation_failed = 1000 ,
    
} PolyErrors;