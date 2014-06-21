#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Temporary
#include <getopt.h>
//#include "stack.h"
typedef struct
{
	void **content;
	int size;
	int top;
} Stack;
void stackInit(Stack *s);
void stackPush(Stack *s, void* val);
void* stackTop(Stack *s);
void* stackPop(Stack *s);
int stackSize(Stack *s);
void stackFree(Stack *s);

#define bool char
#define true 1
#define false 0

#define PI 3.141592653589793
#ifdef REAL
#define klee_make_symbolic(x,y,z)   
#endif

typedef enum
{
	addop,
	multop,
	expop,
	lparen,
	rparen,
	digit,
	value,
	decimal,
	space,
	text,
	function,
	identifier,
	argsep,
	invalid
} Symbol;

struct Preferences
{
	struct Display
	{
		bool tokens;
		bool postfix;
	} display;
	struct Mode
	{
		bool degrees;
	} mode;
} prefs;

typedef enum
{
	divZero,
	overflow,
	parenMismatch
} Error;

typedef char* token;

typedef int number;

#ifdef REAL
#define klee_detect_int(x,y,z) 
#endif
// only use in real calc
char* ufgets(FILE* stream) {
    unsigned int maxlen = 128, size = 128;
    char* buffer = (char*)malloc(maxlen);
    if(buffer != NULL) {
        char ch = EOF;
        int pos = 0;

        while((ch = getchar()) != EOF && ch != '\n') {
            buffer[pos++] = ch;
            if(pos == size) {
                size = pos + maxlen;
                buffer = (char*)realloc(buffer, size); } }
        buffer[pos] = '\0'; }
    return buffer; }
