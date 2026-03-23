#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "polynom.h"
#include "integer.h"
#include "double.h"
#include "prosessing.h"
#include "PolynomErrors.h"
#include "windows.h"

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
    PolyErrors err;
    const TypeInfo *t = get_int_type_info();
    Polynom *p = create_poly(t, degree, &err);
    assert(err == ok);
    assert(p != NULL);
    for (size_t i = 0; i <= degree; i++)
        poly_set_coef(p, i, &coeffs[i]);
    return p;
}

static Polynom *create_double_poly(double *coeffs, size_t degree)
{
    PolyErrors err;
    const TypeInfo *t = get_double_type_info();
    Polynom *p = create_poly(t, degree, &err);
    assert(err == ok);
    assert(p != NULL);
    for (size_t i = 0; i <= degree; i++)
        poly_set_coef(p, i, &coeffs[i]);
    return p;
}


static void test_create_zero_degree()
{
    puts("Создание полинома степени 0");
    PolyErrors err;
    const TypeInfo *t = get_int_type_info();
    Polynom *p = create_poly(t, 0, &err);

    assert(err == ok);
    assert(p != NULL);
    assert(poly_get_degree(p) == 0);
    check_coef(p, 0, 0.0);
    delete_poly(p);
}

static void test_create_large_degree()
{
    puts("Создание полинома большой степени (100)");
    PolyErrors err;
    const TypeInfo *t = get_double_type_info();
    Polynom *p = create_poly(t, 100, &err);

    assert(err == ok);
    assert(p != NULL);
    assert(poly_get_degree(p) == 100);
    for (size_t i = 0; i <= 100; i++)
        check_coef(p, i, 0.0);
    delete_poly(p);
}

static void test_create_null_info()
{
    puts("Создание с NULL TypeInfo");
    PolyErrors err;
    Polynom *p = create_poly(NULL, 5, &err);

    assert(p == NULL);
    assert(err == null_error);
}

static void test_delete_null()
{
    puts("Удаление NULL");
    delete_poly(NULL);
}


static void test_get_set_coef()
{
    puts("Установка и получение коэффициентов");
    int val = 42;
    Polynom *p = create_int_poly(&val, 0);

    int new_val = 100;
    poly_set_coef(p, 0, &new_val);

    const void *res = poly_get_coef(p, 0);
    assert(res != NULL);
    assert(*(const int *)res == 100);
    assert(poly_get_coef(p, 1) == NULL);
    assert(poly_get_coef(NULL, 0) == NULL);

    delete_poly(p);
}

static void test_get_degree_edge()
{
    puts("Степень NULL");
    assert(poly_get_degree(NULL) == 0);
}


static void test_add_same_degree()
{
    puts("Сложение одинаковой степени");
    int c1[] = {1, 2, 3};
    int c2[] = {4, 5, 6};
    Polynom *p1 = create_int_poly(c1, 2);
    Polynom *p2 = create_int_poly(c2, 2);

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 2, &err);
    assert(res != NULL);

    err = add_poly(p1, p2, res);
    assert(err == ok);
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
    puts("Сложение разной степени");
    int c1[] = {1, 2};
    int c2[] = {3, 4, 5, 6};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 3);

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 3, &err);
    assert(res != NULL);

    err = add_poly(p1, p2, res);
    assert(err == ok);
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
    puts("Сложение противоположных");
    int c1[] = {5, -5};
    int c2[] = {-5, 5};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 1);

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 1, &err);
    assert(res != NULL);

    err = add_poly(p1, p2, res);
    assert(err == ok);
    check_coef(res, 0, 0.0);
    check_coef(res, 1, 0.0);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}

static void test_add_null_args()
{
    puts("Сложение с NULL аргументами");
    PolyErrors err;
    Polynom *dummy = create_int_poly((int[]){1}, 0);
    Polynom *res = create_int_poly((int[]){1}, 0);

    err = add_poly(NULL, NULL, res);
    assert(err == null_error);

    err = add_poly(dummy, NULL, res);
    assert(err == null_error);

    err = add_poly(NULL, dummy, res);
    assert(err == null_error);
    Polynom *d_double = create_double_poly((double[]){1.0}, 0);
    err = add_poly(dummy, d_double, res);
    assert(err == type_error);

    delete_poly(dummy);
    delete_poly(d_double);
    delete_poly(res);
}

static void test_add_dimension_mismatch()
{
    puts("Ошибка размерности результата");
    int c1[] = {1, 2};
    int c2[] = {3, 4};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 1);
    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 2, &err);

    err = add_poly(p1, p2, res);
    assert(err == degree_error);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}


static void test_mul_simple()
{
    puts("Умножение простых");
    int c1[] = {0, 1};
    int c2[] = {0, 1};
    Polynom *p1 = create_int_poly(c1, 1);
    Polynom *p2 = create_int_poly(c2, 1);

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 2, &err);
    assert(res != NULL);

    err = mul_poly(p1, p2, res);
    assert(err == ok);
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
    puts("Умножение на нулевой");
    int c1[] = {1, 2, 3};
    Polynom *p1 = create_int_poly(c1, 2);
    Polynom *zero = create_poly(get_int_type_info(), 2, NULL);

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 4, &err); 
    assert(res != NULL);

    err = mul_poly(p1, zero, res);
    assert(err == ok);
    for (size_t i = 0; i <= poly_get_degree(res); i++)
        check_coef(res, i, 0.0);

    delete_poly(p1);
    delete_poly(zero);
    delete_poly(res);
}

static void test_mul_constants()
{
    puts("Умножение констант");
    int v1 = 2, v2 = 3;
    Polynom *p1 = create_int_poly(&v1, 0);
    Polynom *p2 = create_int_poly(&v2, 0);

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 0, &err);
    assert(res != NULL);

    err = mul_poly(p1, p2, res);
    assert(err == ok);
    assert(poly_get_degree(res) == 0);
    check_coef(res, 0, 6.0);

    delete_poly(p1);
    delete_poly(p2);
    delete_poly(res);
}

static void test_mul_null_args()
{
    puts("Умножение с NULL аргументами");
    PolyErrors err;
    Polynom *dummy = create_int_poly((int[]){1}, 0);
    Polynom *res = create_int_poly((int[]){1}, 0);

    err = mul_poly(NULL, dummy, res);
    assert(err == null_error);

    err = mul_poly(dummy, NULL, res);
    assert(err == null_error);

    delete_poly(dummy);
    delete_poly(res);
}


static void test_scalar_mul_positive()
{
    puts("Умножение на положительный скаляр");
    int c[] = {1, 2};
    Polynom *p = create_int_poly(c, 1);
    int s = 3;

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 1, &err);
    assert(res != NULL);

    err = scalar_mul_poly(p, &s, res);
    assert(err == ok);
    check_coef(res, 0, 3.0);
    check_coef(res, 1, 6.0);

    delete_poly(p);
    delete_poly(res);
}

static void test_scalar_mul_negative()
{
    puts("Умножение на отрицательный скаляр");
    int c[] = {2, -4};
    Polynom *p = create_int_poly(c, 1);
    int s = -1;

    PolyErrors err;
    Polynom *res = create_poly(get_int_type_info(), 1, &err);
    assert(res != NULL);

    err = scalar_mul_poly(p, &s, res);
    assert(err == ok);
    check_coef(res, 0, -2.0);
    check_coef(res, 1, 4.0);

    delete_poly(p);
    delete_poly(res);
}

static void test_scalar_null_args()
{
    puts("Скаляр с NULL аргументами");
    PolyErrors err;
    Polynom *dummy = create_int_poly((int[]){1}, 0);
    Polynom *res = create_int_poly((int[]){1}, 0);
    int s = 2;

    err = scalar_mul_poly(NULL, &s, res);
    assert(err == null_error);

    err = scalar_mul_poly(dummy, NULL, res);
    assert(err == null_error);

    delete_poly(dummy);
    delete_poly(res);
}


static void test_eval_horner()
{
    puts("Схема Горнера");
    int c[] = {2, 3, 1};
    Polynom *p = create_int_poly(c, 2);
    int x = 2;

    PolyErrors err;
    const TypeInfo *t = get_int_type_info();
    int *res = (int *)t->create();
    assert(res != NULL);

    err = poly_eval(p, &x, res);
    assert(err == ok);
    assert(*res == 12);

    t->free(res);
    delete_poly(p);
}

static void test_eval_double()
{
    puts("Вычисление на double");
    double c[] = {1.5, 2.5};
    Polynom *p = create_double_poly(c, 1);
    double x = 2.0;

    PolyErrors err;
    const TypeInfo *t = get_double_type_info();
    double *res = (double *)t->create();

    err = poly_eval(p, &x, res);
    assert(err == ok);
    assert(double_eq(*res, 6.5));

    t->free(res);
    delete_poly(p);
}

static void test_eval_null_args()
{
    puts("Вычисление с NULL аргументами");
    PolyErrors err;
    int c[] = {1};
    Polynom *p = create_int_poly(c, 0);
    int x = 1;
    int res_val;

    err = poly_eval(NULL, &x, &res_val);
    assert(err == null_error);

    err = poly_eval(p, NULL, &res_val);
    assert(err == null_error);

    err = poly_eval(p, &x, NULL);
    assert(err == null_error);

    delete_poly(p);
}


static void test_compose_linear()
{
    puts("Композиция линейных");
    int p_c[] = {1, 2}; 
    int q_c[] = {0, 3}; 
    Polynom *P = create_int_poly(p_c, 1);
    Polynom *Q = create_int_poly(q_c, 1);

    PolyErrors err;
    Polynom *R = create_poly(get_int_type_info(), 1, &err);
    assert(R != NULL);

    err = compose_poly(P, Q, R);
    assert(err == ok);
    assert(poly_get_degree(R) == 1);
    check_coef(R, 0, 1.0);
    check_coef(R, 1, 6.0);

    delete_poly(P);
    delete_poly(Q);
    delete_poly(R);
}

static void test_compose_with_zero_poly()
{
    puts("Композиция с константой");
    int p_c[] = {5, 2};
    Polynom *P = create_int_poly(p_c, 1);
    int zero_val = 0;
    Polynom *Z = create_int_poly(&zero_val, 0);

    PolyErrors err;
    Polynom *R = create_poly(get_int_type_info(), 0, &err);
    assert(R != NULL);

    err = compose_poly(P, Z, R);
    assert(err == ok);
    assert(poly_get_degree(R) == 0);
    check_coef(R, 0, 5.0);

    delete_poly(P);
    delete_poly(Z);
    delete_poly(R);
}

static void test_compose_null_args()
{
    puts("Композиция с NULL");
    PolyErrors err;
    Polynom *dummy = create_int_poly((int[]){1}, 0);
    Polynom *res = create_int_poly((int[]){1}, 0);

    err = compose_poly(NULL, dummy, res);
    assert(err == null_error);

    err = compose_poly(dummy, NULL, res);
    assert(err == null_error);

    delete_poly(dummy);
    delete_poly(res);
}


static void test_parse_standard()
{
    puts("Стандартный ввод");
    PolyErrors err;
    Polynom *p = parse_polynomial("2x^2 - 3x + 1", get_int_type_info(), &err);

    assert(err == ok);
    assert(p != NULL);
    check_coef(p, 2, 2.0);
    check_coef(p, 1, -3.0);
    check_coef(p, 0, 1.0);
    delete_poly(p);
}

static void test_parse_no_spaces()
{
    puts("Без пробелов");
    PolyErrors err;
    Polynom *p = parse_polynomial("x^2+x+1", get_int_type_info(), &err);

    assert(err == ok);
    assert(p != NULL);
    check_coef(p, 2, 1.0);
    check_coef(p, 1, 1.0);
    check_coef(p, 0, 1.0);
    delete_poly(p);
}

static void test_parse_only_constant()
{
    puts("Только константа");
    PolyErrors err;
    Polynom *p = parse_polynomial("42", get_int_type_info(), &err);

    assert(err == ok);
    assert(p != NULL);
    assert(poly_get_degree(p) == 0);
    check_coef(p, 0, 42.0);
    delete_poly(p);
}

static void test_parse_empty_string()
{
    puts("Пустая строка");
    PolyErrors err;
    Polynom *p = parse_polynomial("", get_int_type_info(), &err);

    assert(p == NULL);
    assert(err == poli_parse_errore);
}

static void test_parse_null_input()
{
    puts("NULL строка");
    PolyErrors err;
    Polynom *p = parse_polynomial(NULL, get_int_type_info(), &err);

    assert(p == NULL);
    assert(err == null_error);
}

static void test_parse_double_for_int()
{
    puts("Double для Int типа '2.5x'");
    PolyErrors err;
    Polynom *p = parse_polynomial("2.5x", get_int_type_info(), &err);

    assert(p == NULL);
    assert(err == type_error); 
}

static void test_parse_huge_power()
{
    puts("Большая степень");
    PolyErrors err;
    Polynom *p = parse_polynomial("x^1000000000000", get_int_type_info(), &err);

    assert(p == NULL);
    assert(err != ok); 
}

static void test_parse_invalid_chars()
{
    puts("Недопустимые символы 'x^2 + y'");
    PolyErrors err;
    Polynom *p = parse_polynomial("x^2 + y", get_int_type_info(), &err);
    if (p != NULL)
        delete_poly(p);
}


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    puts("Запуск тестов...\n");

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
    test_add_dimension_mismatch();

    puts("\n4. Умножение:");
    test_mul_simple();
    test_mul_by_zero_poly();
    test_mul_constants();
    test_mul_null_args();
    test_scalar_mul_positive();
    test_scalar_mul_negative();
    test_scalar_null_args();

    puts("\n6. Вычисление значения:");
    test_eval_horner();
    test_eval_double();
    test_eval_null_args();

    puts("\n7. Композиция:");
    test_compose_linear();
    test_compose_with_zero_poly();
    test_compose_null_args();

    puts("\n8. Парсер:");
    test_parse_standard();
    test_parse_no_spaces();
    test_parse_only_constant();
    test_parse_empty_string();
    test_parse_null_input();
    test_parse_double_for_int();
    test_parse_huge_power();
    test_parse_invalid_chars();

    puts("\nТесты завершены");
    return 0;
}
