#ifndef SIMPLEINTERPRETER_LABELSTATEMENTHANDLING_H
#define SIMPLEINTERPRETER_LABELSTATEMENTHANDLING_H

#include "../AVLTree.h"
#include "../IDE.h"
#include "InfixCalculator.h"
#include "FunctionsHandling.h"

char *getStatementLabel(char *line);
void labelStatementCase(IDE *ide, char *line, int *index);
int isLabel(char *line);
int isLabelStatement(char *line);

#endif //SIMPLEINTERPRETER_LABELSTATEMENTHANDLING_H
