#include "../Headers/LoadFile.h"

IDE *loadFile(char *fileDirection) {
    FILE *dir = fopen(fileDirection, "r");
    if (!dir)
        return NULL;

    IDE *ide = ideInitialization(25);
    char fileCurrentChar = fgetc(dir);

    String *myString = stringInitialization(100);
    while (fileCurrentChar != EOF) {
        if (fileCurrentChar == '\n') {

            if (stringGetLength(myString) != 0) {
                ideAddMyStringLine(ide, myString);
                destroyString(myString);
                myString = stringInitialization(25);
            }

        }

        else if (fileCurrentChar != '\r')
            stringAddCharAtLast(myString, fileCurrentChar);

        fileCurrentChar = fgetc(dir);
    }

    if (stringGetLength(myString) != 0)
        ideAddMyStringLine(ide, myString);

    destroyString(myString);

    fclose(dir);
    return ide;
}