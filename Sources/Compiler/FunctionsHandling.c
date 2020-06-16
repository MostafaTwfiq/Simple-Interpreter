#include "../../Headers/Compiler/FunctionsHandling.h"

int isFunctionEnd(char *line) {
    trim(line);
    return !strcmp(line, "endFunction");
}

void compileFunctionScope(AVLTree *tree, IDE *ide) {
    Stack *ifConditionsStack = stackInitialization();

    for (int i = 0; i < ideNumberOfLines(ide); i++) {

        char *line = ideGetCharArrayAtIndex(ide, i);

        if (isComment(line))
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

void functionCallieCase(IDE *ide, char *line, FunctionData *functionData, int *lineIndex, Stack *functionsStack) {

    for (int i = 0; i < stackCGetLength(functionData->parametersStack); i++) {

        for (int j = i + 1; j < stackCGetLength(functionData->parametersStack); j++) {
            if (!strcmp(functionData->parametersStack->arr[i], functionData->parametersStack->arr[j])) {
                destroyStackC(functionData->parametersStack);
                destroyString(functionData->functionName);
                free(functionData);

                printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
                return;
            }

        }

    }

    char *functionName = stringToArrayOfCharacters(functionData->functionName);
    int endIndex = getFunctionEndIndex(ide, *lineIndex);

    if (endIndex < 0) {
        free(functionName);

        destroyStackC(functionData->parametersStack);
        destroyString(functionData->functionName);
        free(functionData);

        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        return;
    }

    free(functionName);

    destroyStackC(functionData->parametersStack);
    destroyString(functionData->functionName);
    free(functionData);

    Item item;
    item.cData = 'f';
    stackPush(functionsStack, item);
    *lineIndex = endIndex - 1;
}

void functionCallerCase(AVLTree *tree, IDE *ide, char *line, FunctionData *callerData) {
    char *functionName = stringToArrayOfCharacters(callerData->functionName);
    int startIndex = getFunctionStartIndex(ide, functionName);
    int endIndex = getFunctionEndIndex(ide, startIndex);
    IDE *functionIde = getFunctionLines(ide, startIndex, endIndex);

    if (functionIde == NULL) {
        free(functionName);

        destroyStackC(callerData->parametersStack);
        destroyString(callerData->functionName);
        free(callerData);

        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        return;
    }

    char *callieFunctionLine = ideGetCharArrayAtIndex(ide, startIndex);
    FunctionData *callieData = isFunction(callieFunctionLine);
    free(callieFunctionLine);
    if (stackCGetLength(callerData->parametersStack) != stackCGetLength(callieData->parametersStack)) {

        free(functionName);

        destroyStackC(callerData->parametersStack);
        destroyString(callerData->functionName);
        free(callerData);

        destroyStackC(callieData->parametersStack);
        destroyString(callieData->functionName);
        free(callieData);

        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);

        return;
    }

    int errorFlag = 0;
    StackC *callieParametersTempStack = stackCInitialization();

    while (!stackCIsEmpty(callerData->parametersStack)) {
        char *callerParameter = stackCPop(callerData->parametersStack);
        char *callieParameter = stackCPop(callieData->parametersStack);
        if (!strcmp(callerParameter, callieParameter)) {
            errorFlag = 1;
            free(callerParameter);
            free(callieParameter);
            break;
        }
        else if (isNumber(callerParameter)) {
            char *type = isIntegerNumber(callerParameter) ? "int" : "float";
            NodeValue callerParameterNodeValue;
            callerParameterNodeValue.data = atof(callerParameter);
            strcpy(callerParameterNodeValue.type, type);
            tree->root = insert(tree->root, callieParameter, callerParameterNodeValue);
        } else {
            NodeValue callerParameterNodeValue = BSGetNodeValue(tree->root, callerParameter);
            if (*callerParameterNodeValue.type == '\0') {
                errorFlag = 1;
                free(callerParameter);
                free(callieParameter);
                break;
            }
            tree->root = insert(tree->root, callieParameter, callerParameterNodeValue);
        }

        stackCPush(callieParametersTempStack, callieParameter);
        free(callerParameter);
    }

    if (errorFlag) {

        while (!stackCIsEmpty(callieParametersTempStack)) {
            char *parameterKey = stackCPop(callieParametersTempStack);
            tree->root = BSRemoveNode(tree->root, parameterKey);
            free(parameterKey);
        }

        free(functionName);

        destroyStackC(callerData->parametersStack);
        destroyString(callerData->functionName);
        free(callerData);

        destroyStackC(callieData->parametersStack);
        destroyString(callieData->functionName);
        free(callieData);

        destroyStackC(callieParametersTempStack);

        printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
        return;
    }

    compileFunctionScope(tree, functionIde);

    while (!stackCIsEmpty(callieParametersTempStack)) {
        char *parameterKey = stackCPop(callieParametersTempStack);
        tree->root = BSRemoveNode(tree->root, parameterKey);
        free(parameterKey);
    }

    destroyStackC(callieParametersTempStack);
}

int isFunctionCallie(char *line) {
    int lineLength = strlen(line);
    char word[] = "function ";
    if (lineLength < 12 || line[lineLength - 1] != ':')
        return 0;

    for (int i = 0; i < 9; i++) {
        if (word[i] != line[i])
            return 0;

    }

    return 1;
}

FunctionData *isFunction(char *line) {
    trim(line);
    FunctionData *functionData = (FunctionData *) malloc(sizeof(FunctionData));
    functionData->functionName = stringInitialization(10);
    functionData->parametersStack = stackCInitialization();

    functionData->callingFunctionSignature = !isFunctionCallie(line);

    while (*line != '(' && *line != '\0') {
        stringAddCharAtLast(functionData->functionName, *line);
        line++;
    }

    stringTrim(functionData->functionName);

    if (!functionData->callingFunctionSignature) {
        char *functionNameWithoutFunctionKeyWord =
                stringGetStringBetweenTwoIndices(functionData->functionName, 9,
                                                 stringGetLength(functionData->functionName) - 1);

        stringChangeStringByCharArray(functionData->functionName, functionNameWithoutFunctionKeyWord);

        stringTrim(functionData->functionName);

    }

    char *functionNameArrayOfCharacters = stringToArrayOfCharacters(functionData->functionName);
    if (!isLegalVariableName(functionNameArrayOfCharacters)) {
        free(functionNameArrayOfCharacters);
        destroyString(functionData->functionName);
        destroyStackC(functionData->parametersStack);
        free(functionData);
        return NULL;
    }


    int errorFlag = 0;
    Stack *parenthesisStack = stackInitialization();
    int openParenthesisCounter = 0;
    int commaCounter = 0;

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '(') {
            stackPush(parenthesisStack, (Item) {0, '('});
            openParenthesisCounter++;
        }
        else if (line[i] == ')') {
            stackPop(parenthesisStack);
        }
        else if (isLegalVariableNameCharacter(line + i)) {
            variableCase(functionData->parametersStack, line + i, &i);
            if (!functionData->callingFunctionSignature && !isLegalVariableName(stackCPeek(functionData->parametersStack))) {
                errorFlag = 1;
                break;
            } else if (     functionData->callingFunctionSignature
                            &&  !isLegalVariableName(stackCPeek(functionData->parametersStack))
                            &&  !isNumber(stackCPeek(functionData->parametersStack))      ) {
                errorFlag = 1;
                break;
            }

        }
        else if (line[i] == ',') {
            commaCounter++;

        }
        else if (line[i] == ':' && i == strlen(line) - 1) {
            continue;
        }
        else if (line[i] == ' ') {
            continue;
        } else {
            errorFlag = 1;
            break;
        }

    }

    if (  errorFlag || !stackIsEmpty(parenthesisStack)
          || openParenthesisCounter != 1
          || !( (stackCGetLength(functionData->parametersStack) == 0 && commaCounter == 0)
                || stackCGetLength(functionData->parametersStack) - 1 == commaCounter )  ) {

        destroyString(functionData->functionName);
        destroyStackC(functionData->parametersStack);
        free(functionData);
        return NULL;
    }

    return functionData;
}

IDE *getFunctionLines(IDE *ide, int startIndex, int endIndex) {
    if (startIndex < 0 || endIndex < 0 || startIndex >= endIndex)
        return NULL;

    IDE *functionIDE = ideInitialization(10);
    for (int i = startIndex + 1; i < endIndex; i++) {
        char *line = ideGetCharArrayAtIndex(ide, i);
        ideAddCharArrayLine(functionIDE, line);
        free(line);
    }

    return functionIDE;
}

int getFunctionStartIndex(IDE *ide, char *functionName) {
    for (int i = 0; i < ideNumberOfLines(ide); i++) {
        char *charArrayLine = ideGetCharArrayAtIndex(ide, i);
        FunctionData *functionData = isFunction(charArrayLine);
        char *currentFunctionName = functionData != NULL ? stringToArrayOfCharacters(functionData->functionName) : NULL;
        if (functionData != NULL && !functionData->callingFunctionSignature && !strcmp(functionName, currentFunctionName)) {
            free(charArrayLine);
            free(currentFunctionName);
            destroyString(functionData->functionName);
            destroyStackC(functionData->parametersStack);
            free(functionData);
            return i;
        }

        if (functionData != NULL) {
            free(charArrayLine);
            free(currentFunctionName);
            destroyString(functionData->functionName);
            destroyStackC(functionData->parametersStack);
        }
        free(functionData);

    }

    return -1;

}

int getFunctionEndIndex(IDE *ide, int startIndex) {
    if (startIndex < 0)
        return -1;

    for (int i = startIndex + 1; i < ideNumberOfLines(ide); i++) {
        char *line = ideGetCharArrayAtIndex(ide, i);
        trim(line);

        if (!strcmp(line, "endFunction")) {
            free(line);
            return i;
        }

        free(line);
    }

    return -1;
}