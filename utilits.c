#include <stdio.h>
#include "utilits.h"

void std_clear(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}