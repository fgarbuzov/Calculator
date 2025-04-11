#include "general.h"
#include "stack.h"

int IsEmpty(stack_t* stack)
{
  if (stack == NULL)
    return 1;
  return 0;
}

error_t push(stack_t** pStack, void* pData, size_t dataSize)
{
  stack_t* tmp;
  tmp = (stack_t*) malloc(sizeof(stack_t));
  if (tmp == NULL)
    return MEMORY_SHORTAGE_WHILE_CALC;
  tmp->pData = malloc(dataSize);
  if (tmp->pData == NULL)
    return MEMORY_SHORTAGE_WHILE_CALC;
  memcpy(tmp->pData, pData, dataSize);
  tmp->next = *pStack;
  *pStack = tmp;
  return NO_ERROR;
}

void pop(stack_t** pStack)
{
  if (!IsEmpty(*pStack))
  {
    stack_t* tmp = *pStack;
    *pStack = (*pStack)->next;
    free(tmp->pData);
    free(tmp);
  }
}

void* GetTop(stack_t *top)
{
  if (!IsEmpty(top)) 
    return top->pData;
  return NULL;
}

void clear(stack_t **pStack)
{
  while (!IsEmpty(*pStack))
    pop(pStack);
}