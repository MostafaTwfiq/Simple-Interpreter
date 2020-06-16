#include "../../Headers/Compiler/LabelStatementHandling.h"

char *getStatementLabel(char *line) {
    int statementLabelLength = strlen(line + 5);
    char *statementLabel = (char *) malloc(sizeof(char) * (statementLabelLength + 1) );
    for (int i = 5; i <= statementLabelLength + 5; i++)
        statementLabel[i - 5] = line[i];

    trim(statementLabel);

    return statementLabel;
}

void labelStatementCase(IDE *ide, char *line, int *index) {
    char *statementLabel = getStatementLabel(line);
    trim(statementLabel);

    int statementLineIndex = -1;

    for (int i = 0; i < *index; i++) {
        char *tempLine = ideGetCharArrayAtIndex(ide, i);
        trim(tempLine);

        if (!isLabel(tempLine)) {
            free(tempLine);
            continue;
        }
        else {
            tempLine[strlen(tempLine) - 1] = '\0';
            if (!strcmp(tempLine, statementLabel)) {
                free(tempLine);
                free(statementLabel);
                statementLineIndex = i;
                break;
            }
        }

    }

    if (statementLineIndex == -1) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(statementLabel);
        return;
    }

    *index = statementLineIndex;

}

int isLabel(char *line) {
    trim(line);
    int lineLength = strlen(line);
    if (line[lineLength - 1] != ':')
        return 0;

    char *label = (char *) malloc(sizeof(char) * lineLength);
    label[lineLength - 1] = '\0';

    for (int i = 0; i < lineLength - 1; i++)
        label[i] = line[i];

    trim(label);
    if (!isValidExpressionVariable(label)) {
        free(label);
        return 0;
    }

    free(label);
    return 1;
}

int isLabelStatement(char *line) {
    trim(line);

    char keyWord[] = "goto ";
    int lineLength = strlen(line);

    if (lineLength < 6)
        return 0;

    for (int i = 0; i < 5; i++) {
        if (keyWord[i] != line[i])
            return 0;

    }

    char *statementLabel = getStatementLabel(line);

    trim(statementLabel);
    if (!isValidExpressionVariable(statementLabel)) {
        free(statementLabel);
        return 0;
    }

    free(statementLabel);
    return 1;
}