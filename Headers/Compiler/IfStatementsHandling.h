//
// Created by mosta on 5/26/2020.
//

#ifndef SIMPLEINTERPRETER_IFSTATEMENTSHANDLING_H
#define SIMPLEINTERPRETER_IFSTATEMENTSHANDLING_H

#include "InfixCalculator.h"
#include "../StringFunctions.h"
#include "../AVLTree.h"
#include "../IDE.h"
#include "../Stack.h"

int isIfCondition(char *line);
char *getIfConditionExpression(char *line);
void ifConditionCase(AVLTree *tree, IDE *ide, char *line, Stack *ifConditionsStack, int *index);
int isIfConditionClosing(char *line);

#endif //SIMPLEINTERPRETER_IFSTATEMENTSHANDLING_H
