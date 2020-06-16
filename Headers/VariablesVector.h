//
// Created by mosta on 5/26/2020.
//

#ifndef SIMPLEINTERPRETER_VARIABLESVECTOR_H
#define SIMPLEINTERPRETER_VARIABLESVECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct VariableBox {

    char *key;
    char type[6];
    float value;

} VariableBox;

typedef struct VariablesVector {

    VariableBox *arr;
    int count;
    int length;
} VariablesVector;


VariablesVector *variablesVectorInitialization(int initialLength);
void variablesVectorInsert(VariablesVector *vector, VariableBox variableBox);
VariableBox variablesVectorGet(VariablesVector *vector, int index);
void variablesVectorRemove(VariablesVector *vector, int index);
VariableBox *variablesVectorToArray(VariablesVector *vector);
int variablesVectorLength(VariablesVector *vector);
void destroyVariablesVector(VariablesVector *vector);

#endif //SIMPLEINTERPRETER_VARIABLESVECTOR_H
