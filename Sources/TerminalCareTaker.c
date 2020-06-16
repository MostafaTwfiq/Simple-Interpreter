//
// Created by mosta on 6/14/2020.
//

#include "../Headers/TerminalCareTaker.h"

TerminalCareTaker *terminalCareTakerInitialization(int length) {

    TerminalCareTaker *terminalCareTaker = (TerminalCareTaker *) malloc(sizeof(TerminalCareTaker));

    terminalCareTaker->length = length;
    terminalCareTaker->undoMementoCount = 0;
    terminalCareTaker->redoMementoCount = 0;

    terminalCareTaker->undoMementos = (TerminalMemento **) malloc(sizeof(TerminalMemento *) * length);
    terminalCareTaker->redoMementos = (TerminalMemento **) malloc(sizeof(TerminalMemento *) * length);

    return terminalCareTaker;
}

void freeRedoMementoStack(TerminalCareTaker *careTaker) {

    for (int i = 0; i < careTaker->redoMementoCount; i++)
        destroyTerminalMemento(careTaker->redoMementos[i]);

    careTaker->redoMementoCount = 0;

}

void terminalCareTakerUndoStackPush(TerminalCareTaker *careTaker, TerminalMemento *memento) {
    if (careTaker->undoMementoCount == careTaker->length) {
        shiftTerminalCareTaker(careTaker);
        careTaker->undoMementoCount--;
    }

    freeRedoMementoStack(careTaker);

    careTaker->undoMementos[careTaker->undoMementoCount++] = memento;

}

TerminalMemento *terminalCareTakerRedo(TerminalCareTaker *careTaker, TerminalMemento *currentMemento) {
    if (careTaker == NULL || careTaker->redoMementoCount == 0) {

        exit(-11);

    }

    careTaker->undoMementos[careTaker->undoMementoCount++] = currentMemento;

    return careTaker->redoMementos[--careTaker->redoMementoCount];

}

TerminalMemento *terminalCareTakerRedoPeek(TerminalCareTaker *careTaker) {
    if (careTaker == NULL || careTaker->redoMementoCount == 0) {

        exit(-11);

    }

    return careTaker->redoMementos[careTaker->redoMementoCount - 1];

}

TerminalMemento *terminalCareTakerUndoPeek(TerminalCareTaker *careTaker) {
    if (careTaker == NULL || careTaker->undoMementoCount == 0) {

        exit(-11);

    }

    return careTaker->undoMementos[careTaker->undoMementoCount - 1];

}

TerminalMemento *terminalCareTakerUndo(TerminalCareTaker *careTaker, TerminalMemento *currentMemento) {
    if (careTaker->undoMementoCount == 0)
        exit(-11);


    careTaker->redoMementos[careTaker->redoMementoCount++] = currentMemento;

    return careTaker->undoMementos[--careTaker->undoMementoCount];

}

void shiftTerminalCareTaker(TerminalCareTaker *terminalCareTaker) {

    destroyTerminalMemento(terminalCareTaker->undoMementos[0]);

    for (int i = 0; i < terminalCareTaker->length - 1; i++)
        terminalCareTaker->undoMementos[i] = terminalCareTaker->undoMementos[i + 1];

}