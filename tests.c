#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "polynom.h"
#include "integer.h"
#include "double.h"
#include "prosessing.h"

#define EPS 1e-6

static int double_eq(double a, double b)
{
    return fabs(a - b) < EPS;
}

static void check_coef(const Polynom *p, size_t index, double expected_val)
{
    assert(p != NULL);
    const void *val = poly_get_coef(p, index);
    assert(val != NULL);
    if (p->type_info->size == sizeof(int))
    {
        assert(*(const int *)val == (int)expected_val);
    }
    else
    {
        assert(double_eq(*(const double *)val, expected_val));
    }
}

static Polynom *create_int_poly(int *coeffs, size_t degree)
{
    const TypeInfo *t = get_int_type_info();
    Polynom *p = create_poly(t, degree);
    assert(p != NULL);
    for (size_t i = 0; i <= degree; i++)
        poly_set_coef(p, i, &coeffs[i]);
    return p;
}

static Polynom *create_double_poly(double *coeffs, size_t degree)
{
    const TypeInfo *t = get_double_type_info();
    Polynom *p = create_poly(t, degree);
    assert(p != NULL);
    for (size_t i = 0; i <= degree; i++)
        poly_set_coef(p, i, &coeffs[i]);
    return p;
}

static void test_create_zero_degree()
{
    puts("  [CREATE] Создание полинома степени 0");
    const TypeInfo *t = get_int_type_info();
    Polynom *p = create_poly(t, 0);
    assert(p != NULL);
    assert(poly_get_degree(p) == 0);
    check_coef(p, 0, 0.0); 
    delete_poly(p);
}

static void test_create_large_degree()
{
    puts("  [CREATE] Создание полинома большой степени (100)");
    const TypeInfo *t = get_double_type_info();
    Polynom *p = create_poly(t, 100);
    assert(p != NULL);
    assert(poly_get_degree(p) == 100);
    for (size_t i = 0; i <= 100; i++)
        check_coef(p, i, 0.0);
    delete_poly(p);
}

static void test_create_null_info()
{
    puts("  [CREATE] Создание с NULL TypeInfo (должно вернуть NULL)");
    Polynom *p = create_poly(NULL, 5);
    assert(p == NULL);
}

static void test_delete_null()
{
    puts("  [DELETE] Удаление NULL (не должно падать)");
    delete_poly(NULL); 
}


static void test_get_set_coef()
{
    puts("  [ACCESS] Установка и получение коэффициентов");
    int val = 42;
    Polynom *p = create_int_poly(&val, 0);

    int new_val = 100;
    poly_set_coef(p, 0, &new_val);

    const void *res = poly_get_coef(p, 0);
    assert(res != NULL);
    assert(*(const int *)res == 100);

    assert(poly_get_coef(p, 1) == NULL);

    delete_poly(p);
}

static void test_get_degree_edge()
{
    puts("  [ACCESS] Степень NULL полинома");
    assert(poly_get_degree(NULL) == 0);
}


static void test_add_same_degree()
{
    puts("  [ADD] Сложение одинаковой степени");
    int c1[] = {1, 2, 3};
    int c2[] = {4, 5, 6};
    Polynom *p1 = create_int_poly(c1, 2);
    Polynom *p2 = create_int_poly(c2, 2);
    Polynom *res = add_poly(p1, p2);

    assert(poly_get_degree(res) == 2);
    check_coef(res, 0, 5.0); 
    check_coef(res, 1, 7.0); 
    check_coef(res, 2, 9.0); 

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}

static void test_add_different_degree()
{
    puts("  [ADD] Сложение разной степени (большой + маленький)");
    int c1[] = {1, 2};      
    int c2[] = {3, 4, 5, 6};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 3);
    Polynom *res = add_poly(p1, p2);

    assert(poly_get_degree(res) == 3);
    check_coef(res, 0, 4.0);
    check_coef(res, 1, 6.0);
    check_coef(res, 2, 5.0);
    check_coef(res, 3, 6.0);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}

static void test_add_opposite_coeffs()
{
    puts("  [ADD] Сложение противоположных (результат 0)");
    int c1[] = {5, -5};
    int c2[] = {-5, 5};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 1);
    Polynom *res = add_poly(p1, p2);

    check_coef(res, 0, 0.0);
    check_coef(res, 1, 0.0);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}

static void test_add_null_args()
{
    puts("  [ADD] Сложение с NULL аргументами");
    assert(add_poly(NULL, NULL) == NULL);
    int c[] = {1};
    Polynom *p = create_int_poly(c, 0);
    assert(add_poly(p, NULL) == NULL);
    assert(add_poly(NULL, p) == NULL);
    delete_poly(p);
}


static void test_mul_simple()
{
    puts("  [MUL] Умножение простых полиномов (x * x = x^2)");
    int c1[] = {0, 1}; 
    int c2[] = {0, 1};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 1);
    Polynom *res = mul_poly(p1, p2);

    assert(poly_get_degree(res) == 2);
    check_coef(res, 0, 0.0);
    check_coef(res, 1, 0.0);
    check_coef(res, 2, 1.0);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}

static void test_mul_by_zero_poly()
{
    puts("  [MUL] Умножение на нулевой полином");
    int c1[] = {1, 2, 3};
    Polynom *p1 = create_int_poly(c1, 2);
    const TypeInfo *t = get_int_type_info();
    Polynom *zero = create_poly(t, 2); 

    Polynom *res = mul_poly(p1, zero);
    assert(res != NULL);
    for (size_t i = 0; i <= poly_get_degree(res); i++)
        check_coef(res, i, 0.0);

    delete_poly(p1);
    delete_poly(zero);
    delete_poly(res);
}

static void test_mul_constants()
{
    puts("  [MUL] Умножение констант (2 * 3 = 6)");
    int v1 = 2, v2 = 3;
    Polynom *p1 = create_int_poly(&v1, 0);
    Polynom *p2 = create_int_poly(&v2, 0);
    Polynom *res = mul_poly(p1, p2);

    assert(poly_get_degree(res) == 0);
    check_coef(res, 0, 6.0);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}


static void test_scalar_mul_positive()
{
    puts("  [SCALAR] Умножение на положительный скаляр");
    int c[] = {1, 2};
    Polynom *p = create_int_poly(c, 1);
    int s = 3;
    Polynom *res = scalar_mul_poly(p, &s);

    check_coef(res, 0, 3.0);
    check_coef(res, 1, 6.0);

    delete_poly(p);
    delete_poly(res);
}

static void test_scalar_mul_negative()
{
    puts("  [SCALAR] Умножение на отрицательный скаляр");
    int c[] = {2, -4};
    Polynom *p = create_int_poly(c, 1);
    int s = -1;
    Polynom *res = scalar_mul_poly(p, &s);

    check_coef(res, 0, -2.0);
    check_coef(res, 1, 4.0);

    delete_poly(p);
    delete_poly(res);
}


static void test_eval_horner_scheme()
{
    puts("  [EVAL] Схема Горнера (2 + 3x + x^2 при x=2 -> 2+6+4=12)");
    int c[] = {2, 3, 1};
    Polynom *p = create_int_poly(c, 2);
    int x = 2;
    int *res = (int *)poly_eval(p, &x);
    assert(*res == 12);
    get_int_type_info()->free(res);
    delete_poly(p);
}

static void test_eval_null_args()
{
    puts("  [EVAL] Вычисление с NULL аргументами");
    int c[] = {1};
    Polynom *p = create_int_poly(c, 0);
    assert(poly_eval(NULL, &c[0]) == NULL);
    assert(poly_eval(p, NULL) == NULL);
    delete_poly(p);
}


static void test_compose_linear()
{
    puts("  [COMPOSE] Композиция линейных (P(x)=2x+1, Q(x)=3x -> P(Q)=6x+1)");
    int p_c[] = {1, 2}; 
    int q_c[] = {0, 3};
    Polynom *P = create_int_poly(p_c, 1);
    Polynom *Q = create_int_poly(q_c, 1);

    Polynom *R = compose_poly(P, Q);
    assert(poly_get_degree(R) == 1);
    check_coef(R, 0, 1.0);
    check_coef(R, 1, 6.0);

    delete_poly(P);
    delete_poly(Q);
    delete_poly(R);
}

static void test_compose_with_zero_poly()
{
    puts("  [COMPOSE] Композиция с нулевым полиномом P(0)");
    int p_c[] = {5, 2}; 
    Polynom *P = create_int_poly(p_c, 1);

    int zero = 0;
    Polynom *Z = create_int_poly(&zero, 0);

    Polynom *R = compose_poly(P, Z);
    assert(poly_get_degree(R) == 0);
    check_coef(R, 0, 5.0); 

    delete_poly(P);
    delete_poly(Z);
    delete_poly(R);
}

static void test_parse_standard()
{
    puts("  [PARSE] Стандартный ввод '2x^2 - 3x + 1'");
    Polynom *p = parse_polynomial("2x^2 - 3x + 1", get_int_type_info());
    assert(p != NULL);
    check_coef(p, 2, 2.0);
    check_coef(p, 1, -3.0);
    check_coef(p, 0, 1.0);
    delete_poly(p);
}

static void test_parse_no_spaces()
{
    puts("  [PARSE] Без пробелов 'x^2+x+1'");
    Polynom *p = parse_polynomial("x^2+x+1", get_int_type_info());
    assert(p != NULL);
    check_coef(p, 2, 1.0);
    check_coef(p, 1, 1.0);
    check_coef(p, 0, 1.0);
    delete_poly(p);
}

static void test_parse_only_constant()
{
    puts("  [PARSE] Только константа '42'");
    Polynom *p = parse_polynomial("42", get_int_type_info());
    assert(p != NULL);
    assert(poly_get_degree(p) == 0);
    check_coef(p, 0, 42.0);
    delete_poly(p);
}

static void test_parse_negative_lead()
{
    puts("  [PARSE] Отрицательный лидирующий '-x^3'");
    Polynom *p = parse_polynomial("-x^3", get_int_type_info());
    assert(p != NULL);
    check_coef(p, 3, -1.0);
    delete_poly(p);
}

static void test_parse_empty_string()
{
    puts("  [PARSE] Пустая строка");
    Polynom *p = parse_polynomial("", get_int_type_info());
    assert(p == NULL);
}

static void test_parse_double_for_int()
{
    puts("  [PARSE] Double для Int типа '2.5x'");
    Polynom *p = parse_polynomial("2.5x", get_int_type_info());
    assert(p == NULL); 
}

static void test_parse_huge_power()
{
    puts("  [PARSE] Огромная степень");
    Polynom *p = parse_polynomial("x^1000000000000", get_int_type_info());
    assert(p == NULL);
}


int main()
{
    puts("--- Тесты ---\n");

    puts("1. Создание и удаление:");
    test_create_zero_degree();
    test_create_large_degree();
    test_create_null_info();
    test_delete_null();

    puts("\n2. Доступ к данным:");
    test_get_set_coef();
    test_get_degree_edge();

    puts("\n3. Сложение:");
    test_add_same_degree();
    test_add_different_degree();
    test_add_opposite_coeffs();
    test_add_null_args();

    puts("\n4. Умножение полиномов:");
    test_mul_simple();
    test_mul_by_zero_poly();
    test_mul_constants();

    puts("\n5. Умножение на скаляр:");
    test_scalar_mul_positive();
    test_scalar_mul_negative();

    puts("\n6. Вычисление значения:");
    test_eval_horner_scheme();
    test_eval_null_args();

    puts("\n7. Композиция:");
    test_compose_linear();
    test_compose_with_zero_poly();

    puts("\n8. Парсер (Краевые случаи):");
    test_parse_standard();
    test_parse_no_spaces();
    test_parse_only_constant();
    test_parse_negative_lead();
    test_parse_empty_string();
    test_parse_double_for_int();
    test_parse_huge_power();

    puts("\nТесты пройдены");
    return 0;
}