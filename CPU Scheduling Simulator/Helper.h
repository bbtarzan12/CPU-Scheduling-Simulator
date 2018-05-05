#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _HUGE_ENUF  1e+300
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define NAN        ((float)(INFINITY * 0.0F))
#define RANDOM_IO 0
#define ARRIVALTIME_I 0
#define ID_I 0

typedef enum { FALSE, TRUE } bool;

void PrintError(char* text);
void PrintWarning(char* text);