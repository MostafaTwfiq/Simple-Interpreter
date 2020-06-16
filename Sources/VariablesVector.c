#include "../Headers/VariablesVector.h"

VariablesVector *variablesVectorInitialization(int initialLength) {
    VariablesVector *vector = (VariablesVector *) malloc(sizeof(VariablesVector));
    vector->length = initialLength;
    vector->count = 0;
    vector->arr = (VariableBox *) malloc(sizeof(VariableBox) * vector->length);

    return vector;
}

void variablesVectorInsert(VariablesVector *vector, VariableBox variableBox) {
    if (vector->count == vector->length) {
        vector->length *= 2;
        vector->arr = (VariableBox *) realloc(vector->arr, sizeof(VariableBox) * vector->length);
    }

    vector->arr[vector->count].key = malloc(sizeof(char) * (strlen(variableBox.key) + 1) );
    strcpy(vector->arr[vector->count].key, variableBox.key);
    strcpy(vector->arr[vector->count].type, variableBox.type);
    vector->arr[vector->count++].value = variableBox.value;
}

VariableBox variablesVectorGet(VariablesVector *vector, int index) {
    if (vector == NULL || index < 0 || index >= vector->length)
        exit(-007);

    return vector->arr[index];
}

void variablesVectorRemove(VariablesVector *vector, int index) {
    if (vector == NULL || index < 0 || index >= vector->length)
        exit(-007);

    free(vector->arr[index].key);
    for (int i = index; i < vector->count - 1; i++) {
        vector->arr[i] = vector->arr[i + 1];
    }

    vector->count--;
}

VariableBox *variablesVectorToArray(VariablesVector *vector) {
    if (vector == NULL || vector->count == 0)
        return NULL;

    VariableBox *array = (VariableBox *) malloc(sizeof(VariableBox) * vector->count);
    for (int i = 0; i < vector->count; i++) {
        array[i].key = (char *) malloc(sizeof(char *) * (strlen(vector->arr[i].key) + 1) );
        strcpy(array[i].key, vector->arr[i].key);
        strcpy(array[i].type, vector->arr[i].type);
        array[i].value = vector->arr[i].value;
    }

    return array;
}

int variablesVectorLength(VariablesVector *vector) {
    return vector->count;
}

void destroyVariablesVector(VariablesVector *vector) {
    for (int i = 0; i < vector->count; i++)
        free(vector->arr[i].key);

    free(vector->arr);
    free(vector);
}