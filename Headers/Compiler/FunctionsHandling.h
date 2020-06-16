#ifndef SIMPLEINTERPRETER_FUNCTIONSHANDLING_H
#define SIMPLEINTERPRETER_FUNCTIONSHANDLING_H

#include "../String.h"
#include "../IDE.h"
#include "../StackC.h"
#include "../Stack.h"
#include "../AVLTree.h"
#include "../IDE.h"
#include "InfixCalculator.h"
#include "../String.h"
#include "InfixCalculator.h"
#include "../StringFunctions.h"
#include "Compiler.h"

typedef struct FunctionData {

    String *functionName;
    int callingFunctionSignature;
    StackC *parametersStack;

} FunctionData;

void compileFunctionScope(AVLTree *tree, IDE *ide);
void functionCallerCase(AVLTree *tree, IDE *ide, char *line, FunctionData *callerData);
FunctionData *isFunction(char *line);
IDE *getFunctionLines(IDE *ide, int startIndex, int endIndex);
int getFunctionStartIndex(IDE *ide, char *functionName);
int getFunctionEndIndex(IDE *ide, int startIndex);
void functionCallieCase(IDE *ide, char *line, FunctionData *functionData, int *lineIndex, Stack *functionsStack);
int isFunctionEnd(char *line);

#endif //SIMPLEINTERPRETER_FUNCTIONSHANDLING_H
