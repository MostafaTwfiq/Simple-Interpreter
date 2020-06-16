#ifndef SIMPLEINTERPRETER_EXPRESSIONSHANDLING_H
#define SIMPLEINTERPRETER_EXPRESSIONSHANDLING_H

#include "../AVLTree.h"
#include "InfixCalculator.h"
#include "FunctionsHandling.h"

int isExpressionCase(char *line);
void expressionCase(AVLTree *tree, char *line);
int getExpressionVariableLength(char *line);
char *getExpressionVariable(char *line);
int isValidExpressionVariable(char *expressionVariable);

#endif //SIMPLEINTERPRETER_EXPRESSIONSHANDLING_H
