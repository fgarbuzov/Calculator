#include "general.h"

int BufferIncrease(void** pBuf, size_t len, size_t size, int factor)
{
  void* tmp;
  tmp = realloc(*pBuf, factor * len * size);
  if (NULL != tmp) 
    *pBuf = tmp;
  else
  	return 0;
  return 1;
}