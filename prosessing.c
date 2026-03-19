#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "PolynomErrors.h"
#include "polynom.h"

int is_number(int c)
{
    return (c >= '0' && c <= '9');
}

int get_degree(const char *str, int **degrees, int *count)
{
    if (str == NULL || degrees == NULL || count == NULL)
    {
        return null_error;
    }

    if (*str == '\0')
    {
        return null_error;
    }
    int degree_amount = 100;
    int *degree_keeping = malloc(degree_amount * sizeof(int));
    if (degree_keeping == NULL)
    {
        return null_error;
    }

    int tcount = 0;
    int is_variable = 0;
    const char *ptr = str;

    while (*ptr != '\0')
    {
        if (*ptr == 'x' || *ptr == 'X')
        {
            is_variable = 1;
            ptr++;

            while (*ptr == ' ')
                ptr++;

            int degree = 0;
            if (*ptr == '^')
            {
                ptr++;
                while (*ptr == ' ')
                    ptr++;
                if (is_number(*ptr))
                {
                    while (is_number(*ptr))
                    {
                        degree = degree * 10 + (*ptr - '0');
                        ptr++;
                    }
                }
                else
                {
                    free(degree_keeping);
                    return invalid_input;
                }
            }
            else if (is_number(*ptr))
            {
                while (is_number(*ptr))
                {
                    degree = degree * 10 + (*ptr - '0');
                    ptr++;
                }
            }
            else
            {
                degree = 1;
            }
            if (tcount < degree_amount)
            {
                degree_keeping[tcount++] = degree;
            }
            else
            {
                degree_amount *= 2;
                int *newDegrees = realloc(degree_keeping, degree_amount * sizeof(int));
                if (newDegrees == NULL)
                {
                    free(degree_keeping);
                    return null_error;
                }
                degree_keeping = newDegrees;
                degree_keeping[tcount++] = degree;
            }
        }
        else
        {
            ptr++;
        }
    }
    if (!is_variable)
    {
        ptr = str;
        while (*ptr == ' ' || *ptr == '+' || *ptr == '-')
            ptr++;

        if (*ptr == '\0')
        {
            free(degree_keeping);
            return null_error;
        }
        if (is_number(*ptr))
        {
            degree_keeping[tcount++] = 0;
        }
        else
        {
            free(degree_keeping);
            return invalid_input;
        }
    }
    *degrees = malloc(tcount * sizeof(int));
    if (*degrees == NULL)
    {
        free(degree_keeping);
        return null_error;
    }

    for (int i = 0; i < tcount; i++)
    {
        (*degrees)[i] = degree_keeping[i];
    }

    *count = tcount;
    free(degree_keeping);

    return ok;
}

int get_max_degree(const char *str)
{
    int *degrees = NULL;
    int count = 0;
    int maxDegree = 0;

    int result = get_degree(str, &degrees, &count);

    if (result != ok)
    {
        return result;
    }

    for (int i = 0; i < count; i++)
    {
        if (degrees[i] > maxDegree)
        {
            maxDegree = degrees[i];
        }
    }

    free(degrees);
    return maxDegree;
}

void sort_degrees(int *degrees, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (degrees[i] < degrees[j])
            {
                int temp = degrees[i];
                degrees[i] = degrees[j];
                degrees[j] = temp;
            }
        }
    }
}

Polynom *parse_polynomial(const char *str, const TypeInfo *type)
{
    if (!str || !type)
        return NULL;

    // 1. Сначала проверяем, есть ли в строке хоть что-то полезное
    int has_valid_term = 0;
    const char *check_ptr = str;
    while (*check_ptr != '\0')
    {
        if (*check_ptr == 'x' || *check_ptr == 'X' || (*check_ptr >= '0' && *check_ptr <= '9'))
        {
            has_valid_term = 1;
            break;
        }
        check_ptr++;
    }

    // Если нет ни цифр, ни 'x' - это мусор (например "ef", "abc")
    if (!has_valid_term)
    {
        printf("Ошибка: в строке нет чисел или переменной x.\n");
        return NULL;
    }

    int max_deg = get_max_degree(str);
    if (max_deg < 0)
        return NULL;

    Polynom *poly = create_poly(type, max_deg);
    if (!poly)
        return NULL;

    const char *ptr = str;
    int terms_parsed = 0; 

    while (*ptr != '\0')
    {
        while (*ptr == ' ')
            ptr++;
        if (*ptr == '\0')
            break;

        int sign = 1;
        if (*ptr == '+')
            ptr++;
        else if (*ptr == '-')
        {
            sign = -1;
            ptr++;
        }

        double coef_val = 0.0;
        int has_coef = 0;
        char *end_ptr;

        coef_val = strtod(ptr, &end_ptr);

        if (end_ptr != ptr)
        {
            has_coef = 1;
            ptr = end_ptr;

            if (type->size == sizeof(int))
            {
                double int_part;
                if (modf(coef_val, &int_part) != 0.0)
                {
                    printf("Ошибка: введен тип double (%.4g) для типа int.\n", coef_val);
                    delete_poly(poly);
                    return NULL;
                }
            }
        }
        else
        {
            has_coef = 0;
            coef_val = 1.0;
        }

        while (*ptr == ' ')
            ptr++;

        int degree = 0;
        int has_variable = 0;

        if (*ptr == 'x' || *ptr == 'X')
        {
            has_variable = 1;
            ptr++;
            while (*ptr == ' ')
                ptr++;
            if (*ptr == '^')
            {
                ptr++;
                while (*ptr == ' ')
                    ptr++;
                if (*ptr >= '0' && *ptr <= '9')
                {
                    char *end;
                    long d = strtol(ptr, &end, 10);
                    if (d < 0 || d > 1000000)
                    {
                        printf("Ошибка: степень %ld слишком большая.\n", d);
                        delete_poly(poly);
                        return NULL;
                    }
                    degree = (int)d;
                    ptr = end;
                }
                else
                {
                    delete_poly(poly);
                    return NULL;
                }
            }
            else
            {
                degree = 1;
            }
        }
        else
        {
            if (!has_coef)
            {
                break;
            }
            degree = 0;
        }

        if (!has_coef)
            coef_val = 1.0;

        coef_val *= sign;

        void *temp = type->create();
        if (type->size == sizeof(int))
            *(int *)temp = (int)coef_val;
        else if (type->size == sizeof(double))
            *(double *)temp = coef_val;

        poly_set_coef(poly, degree, temp);
        type->free(temp);

        terms_parsed++;
    }

    if (terms_parsed == 0)
    {
        printf("Не удалось считать многочлен\n");
        delete_poly(poly);
        return NULL;
    }

    return poly;
}