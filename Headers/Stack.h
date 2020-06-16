//
// Created by mosta on 5/25/2020.
//

#ifndef SIMPLEINTERPRETER_STACK_H
#define SIMPLEINTERPRETER_STACK_H

#include <stdlib.h>

typedef union
{
    float fData;
    char  cData;
} Item;

typedef struct
{
    Item *arr;
    int count;
    int length;
} Stack;

Stack *stackInitialization();
void stackPush(Stack *stack, Item value);
Item stackPop(Stack *stack);
Item stackPeek(Stack *stack);
int stackIsEmpty(Stack *stack);
int stackLength(Stack *stack);
void destroyStack(Stack *stack);

#endif //SIMPLEINTERPRETER_STACK_H
