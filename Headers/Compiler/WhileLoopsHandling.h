#ifndef SIMPLEINTERPRETER_WHILELOOPSHANDLING_H
#define SIMPLEINTERPRETER_WHILELOOPSHANDLING_H

#include <stdio.h>
#include "../../Headers/ANSI_Colors.h"
#include "../../Headers/StringFunctions.h"
#include "../../Headers/Stack.h"
#include "../../Headers/IDE.h"
#include "../../Headers/AVLTree.h"
#include "../../Headers/Compiler/InfixCalculator.h"

int isWhileLoop(char *line);
void whileLoopCase(AVLTree *tree, IDE *ide, char *line, int *startIndex, Stack *loopsStack);
int isLoopEnd(char *line);
void compileLoop(AVLTree *tree, IDE *ide, int startIndex, int endIndex);

#endif //SIMPLEINTERPRETER_WHILELOOPSHANDLING_H
