#include "../Headers/IDETerminal.h"

void openTerminal(IDE *ide) {

    printfIDELines(ide);

    TerminalCareTaker *careTaker = terminalCareTakerInitialization(10);

    String *line = stringInitialization(100);


    printf("%d >", ideNumberOfLines(ide) + 1);
    while(1) {

        stringFGetS(line);

        if (stringGetLength(line) != 0) {
            char *lineCharArray = stringToArrayOfCharacters(line);
            if (!strcasecmp(lineCharArray, "run")) {
                free(lineCharArray);
                break;

            } else if (!strcasecmp(lineCharArray, "edit")) {
                editLineChoice(ide, careTaker);
                printfIDELines(ide);

            } else if (!strcasecmp(lineCharArray, "remove")) {
                removeLineChoice(ide, careTaker);
                printfIDELines(ide);

            } else if (!strcasecmp(lineCharArray, "add")) {
                addLineChoice(ide, careTaker);
                printfIDELines(ide);

            } else if (!strcasecmp(lineCharArray, "undo")) {
                undoChoice(ide, careTaker);
                printfIDELines(ide);

            } else if (!strcasecmp(lineCharArray, "redo")) {
                redoChoice(ide, careTaker);
                printfIDELines(ide);

            } else {
                TerminalMemento *memento = terminalMementoInitialization(line, "add", ideNumberOfLines(ide) + 1);
                terminalCareTakerUndoStackPush(careTaker, memento);

                ideAddMyStringLine(ide, line);
            }

            free(lineCharArray);
            destroyString(line);
            line = stringInitialization(100);
            printf("%d >", ideNumberOfLines(ide) + 1);

        }
    }

    fseek(stdin, 0, SEEK_END);

    printfTerminalChoices(ide);

    destroyString(line);
    ideDestroy(ide);
}

void addLineChoice(IDE *ide, TerminalCareTaker *careTaker) {

    String *lineIndex = stringInitialization(5);
    String *newLine = stringInitialization(25);

    printf(ANSI_COLOR_BOLD_MAGENTA "Please enter the line index:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);
    stringFGetS(lineIndex);

    char *lineNumberCharArray = stringToArrayOfCharacters(lineIndex);
    if (    !isNumber(lineNumberCharArray)
            || !isIntegerNumber(lineNumberCharArray)
            || atoi(lineNumberCharArray) - 1 < 0
            || atoi(lineNumberCharArray) - 1 >= ideNumberOfLines(ide)   ) {

        printf(ANSI_COLOR_BOLD_RED "Invalid line number.\n\n" ANSI_COLOR_RESET);
        destroyString(lineIndex);
        destroyString(newLine);
        free(lineNumberCharArray);
        return;
    }

    printf(ANSI_COLOR_BOLD_MAGENTA "Please enter the new line:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);
    stringFGetS(newLine);

    IDE *tempIDE = ideInitialization(1);
    ideAddMyStringLine(tempIDE, newLine);

    TerminalMemento *memento = terminalMementoInitialization(newLine, "add", atoi(lineNumberCharArray));
    terminalCareTakerUndoStackPush(careTaker, memento);

    ideAddLinesAtIndex(ide, tempIDE, atoi(lineNumberCharArray) - 1);

    ideDestroy(tempIDE);
    destroyString(lineIndex);
    destroyString(newLine);
    free(lineNumberCharArray);

    printf("\n");

}

void removeLineChoice(IDE *ide, TerminalCareTaker *careTaker) {

    String *lineNumber = stringInitialization(5);

    printf(ANSI_COLOR_BOLD_MAGENTA "Please enter the line number:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);
    stringFGetS(lineNumber);

    char *lineNumberCharArray = stringToArrayOfCharacters(lineNumber);
    if (    !isNumber(lineNumberCharArray)
            || !isIntegerNumber(lineNumberCharArray)
            || atoi(lineNumberCharArray) - 1 < 0
            || atoi(lineNumberCharArray) - 1 >= ideNumberOfLines(ide)   ) {

        printf(ANSI_COLOR_BOLD_RED "Invalid line number.\n\n" ANSI_COLOR_RESET);
        destroyString(lineNumber);
        free(lineNumberCharArray);

        return;
    }

    String *line = ideGetMyStringAtIndex(ide, atoi(lineNumberCharArray) - 1);
    TerminalMemento *memento = terminalMementoInitialization(line, "remove", atoi(lineNumberCharArray));
    terminalCareTakerUndoStackPush(careTaker, memento);

    ideRemoveLineAtIndex(ide, atoi(lineNumberCharArray) - 1);

    destroyString(line);
    destroyString(lineNumber);
    free(lineNumberCharArray);

    printf("\n");

}

void editLineChoice(IDE *ide, TerminalCareTaker *careTaker) {

    String *lineNumber = stringInitialization(5);
    String *newLine = stringInitialization(25);

    printf(ANSI_COLOR_BOLD_MAGENTA "Please enter the line number:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);
    stringFGetS(lineNumber);

    char *lineNumberCharArray = stringToArrayOfCharacters(lineNumber);
    if (    !isNumber(lineNumberCharArray)
            || !isIntegerNumber(lineNumberCharArray)
            || atoi(lineNumberCharArray) - 1 < 0
            || atoi(lineNumberCharArray) - 1 >= ideNumberOfLines(ide)   ) {

        printf(ANSI_COLOR_BOLD_RED "Invalid line number.\n\n" ANSI_COLOR_RESET);
        destroyString(lineNumber);
        destroyString(newLine);
        free(lineNumberCharArray);

        printf("\n");
        return;
    }

    printf(ANSI_COLOR_BOLD_MAGENTA "Please enter the new line:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);
    stringFGetS(newLine);

    String *currentLine = ideGetMyStringAtIndex(ide, atoi(lineNumberCharArray) - 1);
    TerminalMemento *memento = terminalMementoInitialization(currentLine, "edit", atoi(lineNumberCharArray));
    terminalCareTakerUndoStackPush(careTaker, memento);

    ideChangeStringLineAtIndex(ide, newLine, atoi(lineNumberCharArray) - 1);

    destroyString(currentLine);
    destroyString(lineNumber);
    destroyString(newLine);
    free(lineNumberCharArray);

    printf("\n");

}

void printfIDELines(IDE *ide) {

    printf(ANSI_COLOR_WHITE "*-------------------------------------*\n" ANSI_COLOR_RESET);

    for (int i = 0; i < ideNumberOfLines(ide); i++) {
        char *line = ideGetCharArrayAtIndex(ide, i);
        printf(ANSI_COLOR_WHITE "%d >" ANSI_COLOR_RESET, i + 1);
        printf(ANSI_COLOR_BLUE "%s\n" ANSI_COLOR_RESET, line);

        free(line);
    }

}

void printfTerminalChoices(IDE *ide) {
    while (1) {
        printf(ANSI_COLOR_BOLD_CYAN "\nPlease choose from the following:\n"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA "1- Compile.\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA "2- Save code.\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA "3- Return back.\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);

        char *choice = (char *) malloc( sizeof(char) * 3 );
        fgets(choice, 3, stdin);
        if (choice[strlen(choice) - 1] == '\n')
            choice[strlen(choice) - 1] = '\0';

        if (!strcmp(choice, "1")) {
            compile(ide);
        }
        else if (!strcmp(choice, "2")) {
            String *fileDirection = stringInitialization(100);
            printf(ANSI_COLOR_BLUE "Please enter the file direction: " ANSI_COLOR_RESET);
            stringFGetS(fileDirection);
            char *fileDirectionArrayOfCharacter = stringToArrayOfCharacters(fileDirection);
            saveCode(ide, fileDirectionArrayOfCharacter);
            free(fileDirectionArrayOfCharacter);
            destroyString(fileDirection);
        } else if (!strcmp(choice, "3")) {
            free(choice);
            return;
        } else {
            printf(ANSI_COLOR_BOLD_RED "Please enter a valid choice.\n" ANSI_COLOR_RESET);
        }

        printf(ANSI_COLOR_BOLD_MAGENTA "Click Enter to continue.\n" ANSI_COLOR_RESET);
        fseek(stdin, 0, SEEK_END);
        fgetc(stdin);
        fseek(stdin, 0, SEEK_END);
        free(choice);

    }
}

void undoChoice(IDE *ide, TerminalCareTaker *careTaker) {

    if (careTaker->undoMementoCount == 0)
        return;

    TerminalMemento *memento;
    TerminalMemento *currentMemento;

    memento = terminalCareTakerUndoPeek(careTaker);

    if (!strcmp(memento->changeType, "add")) {
        memento = terminalCareTakerUndo(careTaker, memento);

        ideRemoveLineAtIndex(ide, memento->lineNumber - 1);

    } else if (!strcmp(memento->changeType, "remove")) {
        memento = terminalCareTakerUndo(careTaker, memento);

        IDE *tempIDE = ideInitialization(1);
        ideAddCharArrayLine(tempIDE, memento->line);

        ideAddLinesAtIndex(ide, tempIDE, memento->lineNumber - 1);

        ideDestroy(tempIDE);

    } else if (!strcmp(memento->changeType, "edit")) {

        String *line = ideGetMyStringAtIndex(ide, memento->lineNumber - 1);
        currentMemento = terminalMementoInitialization(line, "edit", memento->lineNumber);

        memento = terminalCareTakerUndo(careTaker, currentMemento);

        ideChangeCharArrayLineAtIndex(ide, memento->line, memento->lineNumber - 1);

        destroyString(line);
        destroyTerminalMemento(memento);
    }

}

void redoChoice(IDE *ide, TerminalCareTaker *careTaker) {

    if (careTaker->redoMementoCount == 0)
        return;

    TerminalMemento *memento;
    TerminalMemento *currentMemento;

    memento = terminalCareTakerRedoPeek(careTaker);

    if (!strcmp(memento->changeType, "add")) {
        memento = terminalCareTakerRedo(careTaker, memento);

        IDE *tempIDE = ideInitialization(1);
        ideAddCharArrayLine(tempIDE, memento->line);

        ideAddLinesAtIndex(ide, tempIDE, memento->lineNumber - 1);

        ideDestroy(tempIDE);

    } else if (!strcmp(memento->changeType, "remove")) {
        memento = terminalCareTakerRedo(careTaker, memento);

        ideRemoveLineAtIndex(ide, memento->lineNumber - 1);

    } else if (!strcmp(memento->changeType, "edit")) {

        String *line = ideGetMyStringAtIndex(ide, memento->lineNumber - 1);
        currentMemento = terminalMementoInitialization(line, "edit", memento->lineNumber);

        memento = terminalCareTakerRedo(careTaker, currentMemento);

        ideChangeCharArrayLineAtIndex(ide, memento->line, memento->lineNumber - 1);

        destroyString(line);
        destroyTerminalMemento(memento);
    }

}

void saveCode(IDE *ide, char *fileDirection) {
    FILE *dir = fopen(fileDirection, "w+");
    if (!dir) {
        printf(ANSI_COLOR_BOLD_RED "There is something went wrong, the code couldn't been saved!!\n" ANSI_COLOR_RESET);
        return;
    }

    for (int i = 0; i < ideNumberOfLines(ide); i++)
        fprintf(dir, "%s\n", ideGetCharArrayAtIndex(ide, i));

    fclose(dir);
    printf(ANSI_COLOR_GREEN "Done Saving.\n" ANSI_COLOR_RESET);
}