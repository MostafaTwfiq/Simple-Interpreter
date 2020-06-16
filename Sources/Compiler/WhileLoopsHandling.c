#include "../../Headers/Compiler/WhileLoopsHandling.h"
#include "../../Headers/Compiler/FunctionsHandling.h"
#include "../../Headers/Compiler/ForLoopsHandling.h"


void compileLoop(AVLTree *tree, IDE *ide, int startIndex, int endIndex) {
    Stack *ifConditionsStack = stackInitialization();
    Stack *loopsStack = stackInitialization();

    for (int i = startIndex; i < endIndex; i++) {

        char *line = ideGetCharArrayAtIndex(ide, i);

        FunctionData *functionData = isFunction(line);
        if (functionData != NULL) {
            if (functionData->callingFunctionSignature) {
                functionCallerCase(tree, ide, line, functionData);
            } else {
                    printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
                    destroyStackC(functionData->parametersStack);
                destroyString(functionData->functionName);
                    free(functionData);
            }
        }

        else if (isWhileLoop(line)) {
            whileLoopCase(tree, ide, line, &i, loopsStack);
        }
        else if (isLoopEnd(line)) {
            if (stackIsEmpty(loopsStack))
                printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
            else
                stackPop(loopsStack);
        }

        else if (isForLoop(line)) {
            forLoopCase(tree, ide, line, &i, loopsStack);
        }
        else if (isLoopEnd(line)) {
            if (stackIsEmpty(loopsStack))
                printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
            else
                stackPop(loopsStack);
        }

        else if (isComment(line))
            commentCase(line);
        else if (isIfCondition(line))
            ifConditionCase(tree, ide, line, ifConditionsStack, &i);
        else if (isIfConditionClosing(line)) {
            if (stackIsEmpty(ifConditionsStack))
                printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
            else
                stackPop(ifConditionsStack);

        }
        else if (isLabel(line)) {
            free(line);
            continue;
        }
        else if (isLabelStatement(line))
            labelStatementCase(ide, line, &i);
        else if (isExpressionCase(line))
            expressionCase(tree, line);
        else
            printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);

        free(line);
    }

    destroyStack(ifConditionsStack);
}

void whileLoopCase(AVLTree *tree, IDE *ide, char *line, int *startIndex, Stack *loopsStack) {

    if (*startIndex == ideNumberOfLines(ide) - 1) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        return;
    }

    String *conditionString = stringInitialization(10);

    char *linePointer = line + 5;

    while (*linePointer != ':' && *linePointer != '\0') {
        stringAddCharAtLast(conditionString, *linePointer);
        linePointer++;
    }

    char *condition = stringToArrayOfCharacters(conditionString);
    free(conditionString);
    float *ans = calculateInfix(tree, condition);

    if (ans == NULL) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(condition);
        return;
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

        else if (!strcmp(currentLine, "endLoop") && stackIsEmpty(loopsStack)) {
            free(currentLine);
            endIndex = i;
            break;
        }

        else if (!strcmp(currentLine, "endLoop")) {
            stackPop(loopsStack);
        }

        free(currentLine);

    }

    if (endIndex == -1) {
        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        free(condition);
        return;
    }


    while (*ans) {
        free(ans);
        compileLoop(tree, ide, *startIndex + 1, endIndex);
        ans = calculateInfix(tree, condition);
    }

    Item item;
    item.cData = 'l';
    stackPush(loopsStack, item);
    *startIndex = endIndex - 1;

    free(ans);
    free(condition);

}

int isWhileLoop(char *line) {

    if (line == NULL)
        return 0;

    trim(line);

    char keyWord[] = "while";

    for (int i = 0; i < 5 && *line != '\0'; i++) {
        if (keyWord[i] != *line)
            return 0;

        line++;
    }

    if (*line == '\0' || (*line != '(' && *line != ' ') || line[strlen(line) - 1] != ':')
        return 0;

    Stack *openParenthesisStack = stackInitialization();

    while (*line != ':' && *line != '\0') {
        if (*line == '(') {
            Item item;
            item.cData = '(';
            stackPush(openParenthesisStack, item);
        }
        else if (*line == ')') {
            if (stackIsEmpty(openParenthesisStack)) {
                destroyStack(openParenthesisStack);
                return 0;
            }
            stackPop(openParenthesisStack);
        }
        line++;
    }

    destroyStack(openParenthesisStack);
    return 1;
}

int isLoopEnd(char *line) {
    trim(line);
    return !strcmp(line, "endLoop");
}