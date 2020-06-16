//
// Created by mosta on 6/14/2020.
//

#ifndef SIMPLEINTERPRETER_TERMINALCARETAKER_H
#define SIMPLEINTERPRETER_TERMINALCARETAKER_H

#include "TerminalMemento.h"

typedef struct TerminalCareTaker {

    TerminalMemento **undoMementos;
    TerminalMemento **redoMementos;
    int undoMementoCount;
    int redoMementoCount;
    int length;

} TerminalCareTaker;

TerminalCareTaker *terminalCareTakerInitialization(int length);
void terminalCareTakerUndoStackPush(TerminalCareTaker *careTaker, TerminalMemento *memento);
void shiftTerminalCareTaker(TerminalCareTaker *terminalCareTaker);
void freeRedoMementoStack(TerminalCareTaker *careTaker);
TerminalMemento *terminalCareTakerUndo(TerminalCareTaker *careTaker, TerminalMemento *currentMemento);
TerminalMemento *terminalCareTakerRedo(TerminalCareTaker *careTaker, TerminalMemento *currentMemento);
TerminalMemento *terminalCareTakerRedoPeek(TerminalCareTaker *careTaker);
TerminalMemento *terminalCareTakerUndoPeek(TerminalCareTaker *careTaker);

#endif //SIMPLEINTERPRETER_TERMINALCARETAKER_H
