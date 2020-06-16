//
// Created by mosta on 6/14/2020.
//

#include "../Headers/TerminalMemento.h"

TerminalMemento *terminalMementoInitialization(String *line, char *changeType, int lineNumber) {
    TerminalMemento *terminalMemento = (TerminalMemento *) malloc(sizeof(TerminalMemento));

    terminalMemento->line = stringToArrayOfCharacters(line);

    strcpy(terminalMemento->changeType, changeType);

    terminalMemento->lineNumber = lineNumber;

    return terminalMemento;
}

void destroyTerminalMemento(TerminalMemento *memento) {
    free(memento->line);
    free(memento);
}