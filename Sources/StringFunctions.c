#include "../Headers/StringFunctions.h"

void removeAllSpaces(char *infix) {
    if (infix == NULL)
        return;

    for (int i = 0; infix[i] != '\0'; i++) {
        if (infix[i] == ' ') {
            for (int j = i; j < strlen(infix); j++)
                infix[j] = infix[j + 1];

            i--;

        }
    }
}

void trim(char *string) {
    if (string == NULL)
        return;

    while (1) {
        if (*string == ' ') {
            for (int i = 0; i < strlen(string); i++)
                string[i] = string[i + 1];

        } else
            break;

    }

    for (int i = strlen(string) - 1; i >= 0; i--) {

        if (string[i] == ' ')
            string[i] = '\0';
        else
            break;

    }
}

int isInteger(float number) {
    char numberChar[20];
    sprintf(numberChar, "%f", number);

    int index = 0;
    while (numberChar[index] != '.' && numberChar[index] != '\0')
        index++;

    for (int i = index + 1; i < strlen(numberChar); i++) {
        if (numberChar[i] != '0')
            return 0;

    }

    return 1;
}

