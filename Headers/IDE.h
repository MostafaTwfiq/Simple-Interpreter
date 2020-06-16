#ifndef SIMPLEINTERPRETER_IDE_H
#define SIMPLEINTERPRETER_IDE_H

#include "../Headers/String.h"

typedef struct IDE {
    String **lines;
    int count;
    int numberOfLines;

} IDE;

IDE *ideInitialization(int initialNumberOfLines);
void ideAddCharArrayLine(IDE *ide, char *line);
void ideAddMyStringLine(IDE *ide, String *line);
void ideRemoveLineAtIndex(IDE *ide, int index);
int ideGetCharArrayLineIndex(IDE *ide, char *line);
int ideGetMyStringLineIndex(IDE *ide, String *line);
char *ideGetCharArrayAtIndex(IDE *ide, int index);
String *ideGetMyStringAtIndex(IDE *ide, int index);
void ideAddLinesAtIndex(IDE *ide, IDE *linesIDE, int index);
void ideChangeCharArrayLineAtIndex(IDE *ide, char *newLine, int index);
void ideChangeStringLineAtIndex(IDE *ide, String *newLine, int index);
int ideNumberOfLines(IDE *ide);
void ideDestroy(IDE *ide);

#endif //SIMPLEINTERPRETER_IDE_H
