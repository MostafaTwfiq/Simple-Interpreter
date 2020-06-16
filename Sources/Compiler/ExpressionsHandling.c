#include "../../Headers/Compiler/ExpressionsHandling.h"

int isExpressionCase(char *line) {

    for (int i = 0; i < strlen(line); i++) {

        if (isAssignOperator(line + i))
            return 1;

    }

    return 0;
}

void expressionCase(AVLTree *tree, char *line) {
    char *expressionVariable = getExpressionVariable(line);
    if (!isValidExpressionVariable(expressionVariable)) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(expressionVariable);
        return;
    }

    float *answer = calculateInfix(tree, line);
    if (answer == NULL) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(expressionVariable);
        return;
    }

    free(answer);
    free(expressionVariable);
}

int getExpressionVariableLength(char *line) {
    int counter = 0;
    while (!isAssignOperator(line)) {
        counter++;
        line++;
    }
    return counter;
}

char *getExpressionVariable(char *line) {
    char *expressionVariable = (char *) malloc( sizeof(char) * (getExpressionVariableLength(line) + 1) );
    int index = 0;

    while (!isAssignOperator(line)) {
        expressionVariable[index++] = *line;
        line++;
    }

    expressionVariable[index] = '\0';

    return expressionVariable;
}

int isValidExpressionVariable(char *expressionVariable) {
    trim(expressionVariable);

    int expressionVariableLength = strlen(expressionVariable);
    if (expressionVariableLength == 0)
        return 0;

    if (*expressionVariable >= '0' && *expressionVariable <= '9')
        return 0;

    for (int i = 0; i < expressionVariableLength; i++) {
        if (!isLegalVariableNameCharacter(expressionVariable + i))
            return 0;

    }

    return 1;
}