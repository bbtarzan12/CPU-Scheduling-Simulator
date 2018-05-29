#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRIVALTIME_I 0
#define ID_I 1

typedef enum
{
	FALSE, TRUE
} bool;

bool RANDOMIO;

void PrintError(char* text);

void PrintWarning(char * text);
