#include "../../Headers/Compiler/ForLoopsHandling.h"

void forLoopCase(AVLTree *tree, IDE *ide, char *line, int *startIndex, Stack *loopsStack) {
    trim(line);

    if (ideNumberOfLines(ide) == *startIndex + 1)
        return;

    char *linePointer = line;
    String *variableDeclaration = stringInitialization(10);
    String *condition = stringInitialization(10);
    String *updatingStatement = stringInitialization(10);

    while (*linePointer != ' ' && *linePointer != '(')
        linePointer++;

    while (*linePointer != ';') {
        stringAddCharAtLast(variableDeclaration, *linePointer);
        linePointer++;
    }
    linePointer++;
    while (*linePointer != ';') {
        stringAddCharAtLast(condition, *linePointer);
        linePointer++;
    }
    linePointer++;
    while (*linePointer != ':') {
        stringAddCharAtLast(updatingStatement, *linePointer);
        linePointer++;
    }

    int endIndex = -1;
    for (int i = *startIndex + 1; i < ideNumberOfLines(ide); i++) {
        char *currentLine = ideGetCharArrayAtIndex(ide, i);
        trim(currentLine);
        if (isWhileLoop(currentLine) || isForLoop(currentLine)) {
            Item item;
            item.cData = 'l';
            stackPush(loopsStack, item);
        }
        else if (!strcmp(currentLine, "endLoop") && !stackIsEmpty(loopsStack)) {
            stackPop(loopsStack);
        }
        else if (!strcmp(currentLine, "endLoop")) {
            endIndex = i;
            free(currentLine);
            break;
        }

        free(currentLine);

    }

    if (endIndex == -1) {
        destroyString(variableDeclaration);
        destroyString(condition);
        destroyString(updatingStatement);
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        return;
    }

    char *variableDeclarationCharArray = stringToArrayOfCharacters(variableDeclaration);
    char *conditionCharArray = stringToArrayOfCharacters(condition);
    char *updatingStatementCharArray = stringToArrayOfCharacters(updatingStatement);
    destroyString(variableDeclaration);
    destroyString(condition);
    destroyString(updatingStatement);

    AVLTree *tempTree = copyAVLTreeIntoNewTree(tree);
    float *declarationValid = calculateInfix(tempTree, variableDeclarationCharArray);
    float *conditionValid = calculateInfix(tempTree, conditionCharArray);
    float *updatingStatementValid = calculateInfix(tempTree, updatingStatementCharArray);
    if (declarationValid == NULL || conditionValid == NULL || updatingStatementValid == NULL) {
        free(variableDeclarationCharArray);
        free(conditionCharArray);
        free(updatingStatementCharArray);
        avlTreeDestroy(tempTree);
        if (declarationValid != NULL) free(declarationValid);
        if (conditionValid != NULL) free(conditionValid);
        if (updatingStatementValid != NULL) free(updatingStatementValid);
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        return;
    }

    calculateInfix(tree, variableDeclarationCharArray);
    float *conditionAns = calculateInfix(tree, conditionCharArray);
    while (*conditionAns) {
        free(conditionAns);
        compileLoop(tree, ide, *startIndex + 1, endIndex);
        calculateInfix(tree, updatingStatementCharArray);
        conditionAns = calculateInfix(tree, conditionCharArray);
    }

    Item item;
    item.cData = 'l';
    stackPush(loopsStack, item);
    *startIndex = endIndex - 1;

    free(conditionAns);
    free(variableDeclarationCharArray);
    free(conditionCharArray);
    free(updatingStatementCharArray);
    avlTreeDestroy(tempTree);
    free(declarationValid);
    free(conditionValid);
    free(updatingStatementValid);

}

int isForLoop(char *line) {

    trim(line);

    char keyWord[] = "for";
    if (line == NULL || strlen(line) < 5)
        return 0;

    for (int i = 0; i < 3; i++) {
        if (*line != keyWord[i])
            return 0;

        line++;
    }

    if (*line != '(' && *line != ' ' || line[strlen(line) - 1] != ':')
        return 0;


    int semiColonCounter = 0;
    Stack *openParenthesisStack = stackInitialization();

    for (int i = 0; i < strlen(line) - 1; i++) {
        if (line[i] == '(') {
            Item item;
            item.cData = '(';
            stackPush(openParenthesisStack, item);
        }
        else if (line[i] == ')') {
            if (stackIsEmpty(openParenthesisStack)) {
                destroyStack(openParenthesisStack);
                return 0;
            }
            stackPop(openParenthesisStack);
        }
        else if (line[i] == ';')
            semiColonCounter++;

    }

    if (semiColonCounter != 2 || !stackIsEmpty(openParenthesisStack)) {
        destroyStack(openParenthesisStack);
        return 0;
    }

    destroyStack(openParenthesisStack);
    return 1;
}