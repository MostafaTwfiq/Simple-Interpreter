#include "../../Headers/Compiler/IfStatementsHandling.h"

int isIfCondition(char *line) {
    trim(line);
    int lineLength = strlen(line);

    //if :
    if ( !(lineLength > 4 && line[0] == 'i' && line[1] == 'f' && line[2] == ' ') )
        return 0;

    int colonCount = 0;
    for (int i = 0; i < lineLength; i++) {
        if (line[i] == ':')
            colonCount++;

    }

    return colonCount == 1;
}

char *getIfConditionExpression(char *line) {
    int expressionLength = strlen(line) - 4;
    char *expression = (char *) malloc(sizeof(char) * (expressionLength + 1) );
    expression[expressionLength] = '\0';

    int index = 0;
    line += 3;
    while (*line != ':' && *line != '\0') {
        expression[index++] = *line;
        line++;
    }

    return expression;
}

void ifConditionCase(AVLTree *tree, IDE *ide, char *line, Stack *ifConditionsStack, int *index) {
    char *expression = getIfConditionExpression(line);

    float *ans = calculateInfix(tree, expression);
    if (ans == NULL) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(expression);
        return;
    }

    //endif
    int currentLineIndex = *index;
    int tempIndex = -1;
    Stack *tempStack = stackInitialization();
    for (int i = currentLineIndex + 1; i < ideNumberOfLines(ide); i++) {
        char *lineTemp = ideGetCharArrayAtIndex(ide, i);
        trim(lineTemp);
        if (isIfCondition(lineTemp)) {
            Item item;
            item.cData = '{';
            stackPush(tempStack, item);
        }
        else if (!strcmp(lineTemp, "endif") && stackIsEmpty(tempStack)) {
            free(lineTemp);
            tempIndex = i;
            break;
        }
        else if (!strcmp(lineTemp, "endif"))
            stackPop(tempStack);

        free(lineTemp);
    }
    if (tempIndex == -1) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(expression);
        return;
    }

    if (!*ans)
        *index += tempIndex - *index;

    Item item;
    item.cData = '{';
    stackPush(ifConditionsStack, item);

    free(expression);

}

int isIfConditionClosing(char *line) {
    trim(line);
    return !strcmp(line, "endif");
}