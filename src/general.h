#ifndef GENERAL_H_INCLUDED_1799
#define GENERAL_H_INCLUDED_1799

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef enum
{
  NO_ERROR,
  INCORRECT_BRACKETS,
  INVALID_TOKEN,
  INCORRECT_EXPRESSION,
  DIVISION_BY_ZERO,
  NOT_IN_TONERANCE_RANGE,
  MEMORY_SHORTAGE_GENERAL,
  MEMORY_SHORTAGE_WHILE_CALC,
  FAILED_TO_OPEN_FILE,
  TOO_MANY_PARAMETERS,
  NO_EXPRESSION
} error_t;
typedef struct
{
  char name;
  double value;
} dict_t;

error_t Calculate (char const* expression, int len, double* pResult, dict_t* dict, int indDict, char* pVarName);
int BufferIncrease(void** pBuf, size_t len, size_t size, int factor);
//my debug functions
//void* malloc_dbg (size_t size);
void* realloc_dbg (void* ptr, size_t size);

#endif /*GENERAL_H_INCLUDED_1799*/