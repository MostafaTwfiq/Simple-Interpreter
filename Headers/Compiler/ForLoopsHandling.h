#ifndef SIMPLEINTERPRETER_FORLOOPSHANDLING_H
#define SIMPLEINTERPRETER_FORLOOPSHANDLING_H

#include <stdio.h>
#include "../../Headers/ANSI_Colors.h"
#include "../../Headers/StringFunctions.h"
#include "../../Headers/Stack.h"
#include "../../Headers/IDE.h"
#include "../../Headers/AVLTree.h"
#include "../../Headers/Compiler/InfixCalculator.h"
#include "../../Headers/Compiler/WhileLoopsHandling.h"

int isForLoop(char *line);
void forLoopCase(AVLTree *tree, IDE *ide, char *linePointer, int *startIndex, Stack *loopsStack);

#endif //SIMPLEINTERPRETER_FORLOOPSHANDLING_H
