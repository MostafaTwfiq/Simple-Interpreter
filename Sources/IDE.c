#include "../Headers/IDE.h"

IDE *ideInitialization(int initialNumberOfLines) {
    IDE *ide = (IDE *) malloc(sizeof(IDE));
    ide->count = 0;
    ide->numberOfLines = initialNumberOfLines;
    ide->lines = (String **) malloc(sizeof(String *) * initialNumberOfLines);

    return ide;
}

void ideAddCharArrayLine(IDE *ide, char *line) {
    if (ide == NULL || line == NULL)
        exit(-003);

    if (ide->count == ide->numberOfLines) {
        ide->numberOfLines *= 2;
        ide->lines = (String **) realloc(ide->lines, sizeof(String *) * ide->numberOfLines);
    }

    ide->lines[ide->count] = stringInitialization(strlen(line));
    stringChangeStringByCharArray(ide->lines[ide->count++], line);
}

void ideAddMyStringLine(IDE *ide, String *line) {
    if (ide == NULL || line == NULL)
        exit(-003);

    if (ide->count == ide->numberOfLines) {
        ide->numberOfLines *= 2;
        ide->lines = (String **) realloc(ide->lines, sizeof(String) * ide->numberOfLines);
    }

    char *tempLine = stringToArrayOfCharacters(line);
    ide->lines[ide->count] = stringInitialization(stringGetLength(line));
    stringChangeStringByCharArray(ide->lines[ide->count++], tempLine);
    free(tempLine);
}

void ideRemoveLineAtIndex(IDE *ide, int index) {
    if (ide == NULL || index < 0 || ide->count <= index)
        exit(-003);

    destroyString(ide->lines[index]);
    for (int i = index; i < ide->count - 1; i++) {
        ide->lines[i] = ide->lines[i + 1];
    }

    ide->count--;
}

int ideGetCharArrayLineIndex(IDE *ide, char *line) {
    if (ide == NULL || line == NULL)
        exit(-003);

    for (int i = 0; i < ide->count; i++) {
        if (stringIsEqualsToCharArray(ide->lines[i], line))
            return i;

    }

    return -1;

}

int ideGetMyStringLineIndex(IDE *ide, String *line) {
    if (ide == NULL || line == NULL)
        exit(-003);

    for (int i = 0; i < ide->count; i++) {
        if (stringIsEqualsToMyString(ide->lines[i], line))
            return i;

    }

    return -1;

}

char *ideGetCharArrayAtIndex(IDE *ide, int index) {
    if (ide == NULL || index < 0 || ide->count <= index)
        exit(-003);

    return stringToArrayOfCharacters(ide->lines[index]);
}

String *ideGetMyStringAtIndex(IDE *ide, int index) {
    if (ide == NULL || index < 0 || ide->count <= index)
        exit(-003);

    String *myString = stringInitialization(stringGetLength(ide->lines[index]));
    for (int i = 0; i < stringGetLength(ide->lines[index]); i++)
        stringAddCharAtLast(myString, stringGetCharAtIndex(ide->lines[index], i));

    return myString;
}

void ideAddLinesAtIndex(IDE *ide, IDE *linesIDE, int index) {
    if (ide == NULL || linesIDE == NULL || index < 0 || index > ide->count) {
        exit(-003);
    }

    if (ide->numberOfLines - ide->count < linesIDE->count) {
        ide->numberOfLines = (ide->numberOfLines + (linesIDE->count + ide->count - ide->numberOfLines)) * 2;
        ide->lines = (String **) realloc(ide->lines, sizeof(String *) * ide->numberOfLines);
    }

    //  a   b   c   d   e
    //  0   1   2   3   4
    //  k   l   j -->>  index = 1
    //  a   k   l   j   b   c   d   e

    //  a               b   c   d   e
    //  0   1   2   3   4   5   6   7

    for (int i = ide->count - 1; i >= index; i--) //this loop will shift the array to the right from the index value
        ide->lines[i + linesIDE->count] = ide->lines[i];


    for (int i = index; i < index + linesIDE->count; i++) {

        String *string = stringInitialization(stringGetLength(linesIDE->lines[i - index]));

        for (int j = 0; j < stringGetLength(linesIDE->lines[i - index]); j++)
            stringAddCharAtLast(string, stringGetCharAtIndex(linesIDE->lines[i - index], j));

        ide->lines[i] = string;
    }

    ide->count += linesIDE->count;

}

void ideChangeCharArrayLineAtIndex(IDE *ide, char *newLine, int index) {
    if (ide == NULL || newLine == NULL || index < 0 || index >= ideNumberOfLines(ide))
        exit(-003);

    stringChangeStringByCharArray(ide->lines[index], newLine);

}

void ideChangeStringLineAtIndex(IDE *ide, String *newLine, int index) {
    if (ide == NULL || newLine == NULL || index < 0 || index >= ideNumberOfLines(ide))
        exit(-003);

    stringChangeStringByString(ide->lines[index], newLine);

}

int ideNumberOfLines(IDE *ide) {
    if (ide == NULL)
        exit(-003);

    return ide->count;
}

void ideDestroy(IDE *ide) {
    for (int i = 0; i < ide->count; i++)
        destroyString(ide->lines[i]);

    free(ide->lines);
    free(ide);
}