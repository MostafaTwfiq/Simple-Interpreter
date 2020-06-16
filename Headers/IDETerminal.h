//
// Created by mosta on 5/26/2020.
//

#ifndef SIMPLEINTERPRETER_IDETERMINAL_H
#define SIMPLEINTERPRETER_IDETERMINAL_H


#include "../Headers/AVLTree.h"
#include "Compiler/Compiler.h"
#include "IDE.h"
#include "TerminalCareTaker.h"

void openTerminal(IDE *ide);
void printfTerminalChoices(IDE *ide);
void saveCode(IDE *ide, char *fileDirection);

void addLineChoice(IDE *ide, TerminalCareTaker *careTaker);
void removeLineChoice(IDE *ide, TerminalCareTaker *careTaker);
void editLineChoice(IDE *ide, TerminalCareTaker *careTaker);
void printfIDELines(IDE *ide);

void undoChoice(IDE *ide, TerminalCareTaker *careTaker);
void redoChoice(IDE *ide, TerminalCareTaker *careTaker);

#endif //SIMPLEINTERPRETER_IDETERMINAL_H
