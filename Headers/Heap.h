#ifndef SIMPLEINTERPRETER_HEAP_H
#define SIMPLEINTERPRETER_HEAP_H

#include "VariablesVector.h"
#include "ANSI_Colors.h"

void maxheapify(  VariableBox *array, int parent, int sizeofHeap);
VariableBox* buildmaxheap(VariableBox *a, int n);
void displayHeap( VariableBox *a,int n);

#endif //SIMPLEINTERPRETER_HEAP_H
