#ifndef SIMPLEINTERPRETER_COMPILER_H
#define SIMPLEINTERPRETER_COMPILER_H

#include "../AVLTree.h"
#include "../IDE.h"
#include "InfixCalculator.h"
#include "FunctionsHandling.h"
#include "IfStatementsHandling.h"
#include "CommentsHandling.h"
#include "ExpressionsHandling.h"
#include "LabelStatementHandling.h"
#include "WhileLoopsHandling.h"
#include "../Heap.h"
#include "../../Headers/Compiler/ForLoopsHandling.h"

void compile(IDE *ide);
void printingVariablesChoices(AVLTree *tree, VariableBox *heapArray, int heapArrayLength);

#endif //SIMPLEINTERPRETER_COMPILER_H
