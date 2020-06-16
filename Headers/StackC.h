//
// Created by mosta on 5/25/2020.
//

#ifndef SIMPLEINTERPRETER_STACKC_H
#define SIMPLEINTERPRETER_STACKC_H

#include <stdio.h>
#include <stdlib.h>

typedef struct StackC{
    char **arr;
    int count;
    int length;

} StackC;

StackC *stackCInitialization();
void stackCPush(StackC *stack, char *string);
char *stackCPop(StackC *stack);
char *stackCPeek(StackC *stack);
int stackCGetLength(StackC *stack);
int stackCIsEmpty(StackC *stack);
void destroyStackC(StackC *stack);

#endif //SIMPLEINTERPRETER_STACKC_H
