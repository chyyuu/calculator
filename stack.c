#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void stackInit(Stack *s)
{
	s->content = NULL;
	s->size = 0;
	s->top = -1;
}

void stackPush(Stack *s, void* val)
{        klee_detect_int(s->top, 1, 1);//sadd
	if(s->top + 1 >= s->size) // If stack is full
	{
            klee_detect_int(s->size, 1, 1); (s->size)++;
	    klee_detect_int(s->size,sizeof(void*),4); s->content = (void**)realloc(s->content, s->size * sizeof(void*));
	}

	(s->top)++;
	s->content[s->top] = val;
}

void* stackTop(Stack *s)
{
	void *ret = NULL;
	if(s->top >= 0 && s->content != NULL)
		ret = s->content[s->top];
	return ret;
}

void* stackPop(Stack *s)
{
	void *ret = NULL;
	if(s->top >= 0 && s->content != NULL)
		ret = s->content[(s->top)--];
	return ret;
}

int stackSize(Stack *s)
{       klee_detect_int(s->top, 1, 1);
	return s->top + 1;
}

void stackFree(Stack *s)
{
	free(s->content);
	s->content = NULL;
	s->size = 0;
	s->top = -1;
}
