#pragma once 

typedef enum{
    poli_parse_errore=100,
    invalid_input=200,
    null_error=300,
    create_poly_error=400,
    get_coef_error=500,
    type_error=600,
    degree_error=700,
    operation_failed=800,
    ok = 0,
    memory_allocation_failed = 1000 ,
    
} PolyErrors;