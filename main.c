#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polynom.h"
#include "integer.h"
#include "double.h"
#include "prosessing.h"

void clear_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main()
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
        clear_stdin(); 
    }

     if (type_choice == 1)
    {
        type = get_int_type_info();
    }
    else
    {
        type = get_double_type_info();
    }

    Polynom *p1 = NULL;
    Polynom *p2 = NULL;
    int menu = 1;

    while (menu)
    {
        int choice;
        while (1)
        {
            printf("\n--- Меню выбора операции ---\n");
            printf("(1) - Ввести многочлен 1 (формат: 2x^2 - 3x + 1)\n");
            printf("(2) - Ввести многочлен 2\n");
            printf("(3) - Сложение (P1 + P2)\n");
            printf("(4) - Умножение (P1 * P2)\n");
            printf("(5) - Умножение P1 на скаляр\n");
            printf("(6) - Вычислить значение P1 в точке x\n");
            printf("(7) - Композиция P1(P2)\n");
            printf("(8) - Вывести P1 и P2 на экран\n");
            printf("(0) - Выход\n");
            printf("Операция:");

            if (scanf("%d", &choice) == 1 && choice >= 0 && choice <= 8)
            {
                break;
            }

            printf("Некорректный ввод. Dведите число от 0 до 8.\n");
            std_clear(); 
        }

        if (choice == 0)
        {
            break;

        if (choice == 1 || choice == 2)
        {
            char buffer[256];
            printf("Enter polynomial: ");
            if (!fgets(buffer, sizeof(buffer), stdin))
                continue;
            buffer[strcspn(buffer, "\n")] = 0;

            Polynom *p = parse_polynomial(buffer, type);
            if (p)
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
                printf("Polynomial created successfully.\n");
            }
            else
            {
                printf("Невозможность парсинга\n");
            }
        }
        else if (choice >= 3 && choice <= 7)
        {
            if (!p1 || !p2)
            {
                printf("Сначала введите многочлены\n");
                continue;
            }
            Polynom *res = NULL;
            if (choice == 3)
                res = add_poly(p1, p2);
            else if (choice == 4)
                res = mul_poly(p1, p2);
            else if (choice == 5)
            {
                void *sc = type->create();
                printf("Введите скаляр: ");
                type->scan(sc);
                res = scalar_mul_poly(p1, sc);
                type->free(sc);
            }
            else if (choice == 6)
            {
                void *x = type->create();
                printf("Введите скаляр ");
                type->scan(x);
                void *val = poly_eval(p1, x);
                if (val)
                {
                    printf("Результат: ");
                    type->print(val);
                    printf("\n");
                    type->free(val);
                }
                type->free(x);
                continue;
            }
            else if (choice == 7)
                res = compose_poly(p1, p2);

            if (res)
            {
                printf("Результат ");
                poly_print(res);
                delete_poly(res);
            }
            else
            {
                printf("Некорректное поведение программы\n");
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
    return 0;
}
}