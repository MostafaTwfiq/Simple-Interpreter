//
// Created by mosta on 6/14/2020.
//

#ifndef SIMPLEINTERPRETER_TERMINALMEMENTO_H
#define SIMPLEINTERPRETER_TERMINALMEMENTO_H

#include <stdlib.h>
#include "String.h"

typedef struct TerminalMemento {

    char *line;
    int lineNumber;
    char changeType[7];

} TerminalMemento;

TerminalMemento *terminalMementoInitialization(String *line, char *changeType, int lineNumber);
void destroyTerminalMemento(TerminalMemento *memento);

#endif //SIMPLEINTERPRETER_TERMINALMEMENTO_H
