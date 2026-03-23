#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polynom.h"
#include "integer.h"
#include "double.h"
#include "prosessing.h"
#include "utilits.h"
#include "PolynomErrors.h"
#include <windows.h>

int run_main(void)
{
    int type_choice;
    const TypeInfo *type = NULL;

    while (1)
    {
        printf("Выберите тип коэффициентов:\n(1) - int\n(2) - double\nВаш выбор: ");
        if (scanf("%d", &type_choice) == 1 && (type_choice == 1 || type_choice == 2))
        {
            break;
        }
        printf("Некорректный ввод. Пожалуйста, введите 1 или 2.\n");
        std_clear();
    }

    if (type_choice == 1)
        type = get_int_type_info();
    else
        type = get_double_type_info();

    Polynom *p1 = NULL;
    Polynom *p2 = NULL;
    
    int menu = 1;

    while (menu)
    {
        int choice;
        while (1)
        {
            printf("\n--- Меню выбора операции ---\n");
            printf("(1) - Ввести многочлен 1 \n");
            printf("(2) - Ввести многочлен 2\n");
            printf("(3) - Сложение (P1 + P2)\n");
            printf("(4) - Умножение (P1 * P2)\n");
            printf("(5) - Умножение P1 на скаляр\n");
            printf("(6) - Вычислить значение P1 в точке x\n");
            printf("(7) - Композиция P1(P2)\n");
            printf("(8) - Вывести P1 и P2 на экран\n");
            printf("(0) - Выход\n");
            printf("Операция: ");

            if (scanf("%d", &choice) == 1 && choice >= 0 && choice <= 8)
            {
                break;
            }
            printf("Некорректный ввод. Введите число от 0 до 8.\n");
            std_clear();
        }

        if (choice == 0)
            break;

        if (choice == 1 || choice == 2)
        {
            char buffer[256];
            Polynom *p = NULL;
            PolyErrors err;
            int success = 0;

            while (!success)
            {
                std_clear();
                printf("Введите многочлен: ");

                if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                    break;

                buffer[strcspn(buffer, "\n")] = 0;

                p = parse_polynomial(buffer, type, &err);

                if (p != NULL)
                {
                    success = 1;
                }
                else
                {
                    printf("Ошибка парсинга: ");
                    if (err == null_error)
                        printf("Пустой ввод.\n");
                    else if (err == poli_parse_errore)
                        printf("Неверный формат записи.\n");
                    else if (err == memory_allocation_failed)
                        printf("Не хватило памяти.\n");
                    else
                        printf("Код ошибки %d\n", err);

                    printf("Попробуйте еще раз.\n");
                }
            }

            if (success && p != NULL)
            {
                if (choice == 1)
                {
                    if (p1)
                        delete_poly(p1);
                    p1 = p;
                }
                else
                {
                    if (p2)
                        delete_poly(p2);
                    p2 = p;
                }
                printf("Многочлен введен\n");
            }
        }

        else if (choice >= 3 && choice <= 7)
        {
            if (!p1 || !p2)
            {
                printf("Сначала введите оба многочлена\n");
                continue;
            }

            Polynom *res = NULL;
            PolyErrors err = ok;
            size_t res_degree = 0;

            if (choice == 3) 
            {
                size_t res_degree;
                if (p1->degree > p2->degree)
                {
                    res_degree = p1->degree;
                }
                else
                {
                    res_degree = p2->degree;
                }

                res = create_poly(type, res_degree, &err);
                if (res)
                    err = add_poly(p1, p2, res);
            }
            else if (choice == 4)
            {
                res_degree = p1->degree + p2->degree;
                res = create_poly(type, res_degree, &err);
                if (res)
                    err = mul_poly(p1, p2, res);
            }
            else if (choice == 5) 
            {
                void *sc = type->create();
                printf("Введите скаляр: ");
                type->scan(sc);

                res = create_poly(type, p1->degree, &err);
                if (res)
                    err = scalar_mul_poly(p1, sc, res);

                type->free(sc);
            }
            else if (choice == 6) 
            {
                void *x = type->create();
                printf("Введите значение x: ");
                type->scan(x);

                void *val = type->create();
                if (!val)
                {
                    printf("Ошибка выделения памяти для результата.\n");
                    type->free(x);
                    continue;
                }

                err = poly_eval(p1, x, val);

                if (err == ok)
                {
                    double print_value;

                    if (type->size == sizeof(double))
                    {
                        print_value = *(double *)x;
                    }
                    else
                    {
                        print_value = (double)*(int *)x;
                    }

                    printf("Результат P1(%g) = ", print_value);
                    type->print(val);
                    printf("\n");
                }
                else
                {
                    printf("Ошибка вычисления: %d\n", err);
                }

                type->free(val);
                type->free(x);
                continue;
            }
            else if (choice == 7) 
            {
                res_degree = p1->degree * p2->degree;
                res = create_poly(type, res_degree, &err);
                if (res)
                    err = compose_poly(p1, p2, res);
            }

            if (err != ok)
            {
                printf("Ошибка выполнения операции:");
                if (err == null_error)
                    printf("Передан пустой указатель\n");
                else if (err == type_error)
                    printf("Несовместимые типы данных\n");
                else if (err == degree_error)
                    printf("Неверная степень \n");
                else if (err == memory_allocation_failed)
                    printf("Не хватило памяти\n");
                else
                    printf("Код ошибки %d\n", err);

                if (res)
                    delete_poly(res);
            }
            else
            {
                printf("Результат: ");
                poly_print(res);
                delete_poly(res);
            }
        }

        else if (choice == 8)
        {
            if (p1)
            {
                printf("P1 = ");
                poly_print(p1);
            }
            else
                printf("P1 не введен\n");

            if (p2)
            {
                printf("P2 = ");
                poly_print(p2);
            }
            else
                printf("P2 не введен\n");
        }
    }

    if (p1)
        delete_poly(p1);
    if (p2)
        delete_poly(p2);

    printf("Выход из программы.\n");
    return 0;
}