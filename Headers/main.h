#ifndef SIMPLEINTERPRETER_MAIN_H
#define SIMPLEINTERPRETER_MAIN_H

#include <stdio.h>
#include "../Headers/AVLTree.h"
#include "../Headers/IDE.h"
#include "../Headers/LoadFile.h"
#include "Compiler/Compiler.h"
#include "../Headers/IDETerminal.h"

int main();
char *scanChoice();
char *getFileDirectionToLoad();
void printfManual();

#endif //SIMPLEINTERPRETER_MAIN_H
