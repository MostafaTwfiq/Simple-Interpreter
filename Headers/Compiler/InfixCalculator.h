#ifndef SIMPLEINTERPRETER_INFIXCALCULATOR_H
#define SIMPLEINTERPRETER_INFIXCALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../Stack.h"
#include "../AVLTree.h"
#include "../StringFunctions.h"
#include "../StackC.h"

int isAVariableCharacter(char *c);
int isLegalVariableNameCharacter(char *c);
int isOperator(char *c);
int getPriority(char *c);
int isAssignOperator(char *c);
int isRightAssociative(char *c);
int getCaseNumber(StackC *operatorStack,StackC *postfixStack,char *infix,int openParenthesisCount,int notSignCount);
int getOperatorLength(char *c);
int isIntegerNumber(char *number);
void assignVariable(AVLTree *symbolTable,StackC *postfixStack, char *operator, int *errorFlag);
int isNumber(char *string);
int isLegalVariableName(char *variable);
float *getVariableValue(AVLTree *symbolTable,char *variable);
void notOperatorCase(AVLTree *symbolTable,StackC *postfixStack,int *notSignCount,int *errorFlag);
void remainingOperatorsEvaluate(AVLTree *symbolTable,StackC *postfixStack,char *operator,int *errorFlag);
void evaluate(AVLTree *symbolTable,StackC *postfixStack,char *infix,int *notSignCount,int *errorFlag);
void operatorCase(AVLTree *symbolTable,StackC *operatorStack,StackC *postfixStack,char *infix,int openParenthesisCount,int *notSignCount,int *errorFlag);
float *evaluateTheRestOfThePostfix(AVLTree *symbolTable,StackC *operatorStack,StackC *postfixStack,int *notSignCount);
void variableCase(StackC *postfixStack,char *infix,int *index);
float *calculateInfix(AVLTree *symbolTable,char *infix);

#endif //SIMPLEINTERPRETER_INFIXCALCULATOR_H
