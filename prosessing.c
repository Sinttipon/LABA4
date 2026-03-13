#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "PolynomErrors.h"

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
