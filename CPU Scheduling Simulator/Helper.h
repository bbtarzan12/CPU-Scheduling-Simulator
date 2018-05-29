#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _HUGE_ENUF  1e+300
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define NAN        ((float)(INFINITY * 0.0F))
#define RANDOM_IO 1
#define ARRIVALTIME_I 0
#define ID_I 1

typedef enum
{
	FALSE, TRUE
} bool;

inline void PrintError(char* text)
{
	printf("<Error> %s\n", text);
}

inline void PrintWarning(char * text)
{
	printf("<Warning> %s\n", text);
}
