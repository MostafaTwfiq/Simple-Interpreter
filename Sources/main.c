#include "../Headers/main.h"

int main() {
    //system("COLOR");

    printf(ANSI_COLOR_RED "** WELCOME TO A SIMPLE INTERPRETER **\n\n\n" ANSI_COLOR_RESET);

    while(1) {
        char *choice = scanChoice();

        if (!strcmp(choice, "1")) {
            IDE *ide = ideInitialization(10);
            openTerminal(ide);
        }

        else if (!strcmp(choice, "2")) {
            char *fileDirection = getFileDirectionToLoad();
            IDE *ide = loadFile(fileDirection);
            if (ide == NULL) {
                printf(ANSI_COLOR_BOLD_RED "There is something went wrong, the file couldn't open!!\n" ANSI_COLOR_RESET);
            } else {
                free(fileDirection);
                openTerminal(ide);
            }
        }

        else if (!strcmp(choice, "3")) {
            printfManual();
        }

        else if (!strcmp(choice, "4")) {
            free(choice);
            break;
        }

        else
            printf(ANSI_COLOR_BOLD_RED "Please enter a valid choice.\n" ANSI_COLOR_RESET);


        printf(ANSI_COLOR_BOLD_MAGENTA "Click Enter to continue.\n\n" ANSI_COLOR_RESET);
        fseek(stdin, 0, SEEK_END);
        fgetc(stdin);
        fseek(stdin, 0, SEEK_END);
        free(choice);

    }

    return 0;
}

char *scanChoice() {
    printf(ANSI_COLOR_BOLD_CYAN "Please choose from the following:\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA "1- Open terminal.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA "2- Load code.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA "3- Manual.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA "4- Exit.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);

    char *choice = (char *) malloc( sizeof(char) * 3 );

    fgets(choice, 3, stdin);

    if (choice[strlen(choice) - 1] == '\n')
        choice[strlen(choice) - 1] = '\0';

    return choice;
}

char *getFileDirectionToLoad() {
    String *fileDirection = stringInitialization(100);

    printf(ANSI_COLOR_BLUE "Please enter the file direction: " ANSI_COLOR_RESET);
    stringFGetS(fileDirection);
    char *fileDirectionArrayOfCharacter = stringToArrayOfCharacters(fileDirection);

    destroyString(fileDirection);

    return fileDirectionArrayOfCharacter;
}

void printfManual() {
    printf(ANSI_COLOR_RED "\n** Manual **\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "* In the interpreter you has two choices: \n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "1- Start to write a new code that you can compile and save.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "2- Load a ready text file code that you can compile, edit and save.\n\n" ANSI_COLOR_RESET);

    printf(ANSI_COLOR_CYAN "* Some short cuts you need to know while you are writing a code: \n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "1- The key word << EDIT >>\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "     this key word will give you the ability to edit a particular line in the code.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "2- The key word << REMOVE >>\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "     this key word will give you the ability to delete a particular line in the code.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "3- The key word << ADD >>\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "     this key word will give you the ability to add a new line in a particular index in the code.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "4- The key word << UNDO >>\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "     this key word will give you the ability to undo recent changes.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "5- The key word << REDO >>\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "     this key word will give you the ability to redo recent changes.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "6- The key word << RUN >>\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BOLD_CYAN "     you use this key word when you want to compile or save the code.\n" ANSI_COLOR_RESET);

    printf("\n");
}