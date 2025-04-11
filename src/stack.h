#ifndef STACK_H_INCLUDED_1837
#define STACK_H_INCLUDED_1837

struct stack_node
{
	void* pData;
 	struct stack_node* next;
};
typedef struct stack_node stack_t;
int IsEmpty(stack_t*);
error_t push(stack_t**, void*, size_t);
void pop(stack_t**);
void* GetTop(stack_t*);
void clear(stack_t**);

#endif /*STACK_H_INCLUDED_1837*/