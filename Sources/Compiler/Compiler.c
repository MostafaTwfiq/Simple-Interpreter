#include "../../Headers/Compiler/Compiler.h"

void compile(IDE *ide) {
    if (ide == NULL)
        exit(-10);

    Stack *ifConditionsStack = stackInitialization();
    Stack *functionsStack = stackInitialization();
    Stack *loopsStack = stackInitialization();
    IDE *functionsNamesSaver = ideInitialization(10);
    AVLTree *tree = avlTreeInitialization();

    for (int i = 0; i < ideNumberOfLines(ide); i++) {

        char *line = ideGetCharArrayAtIndex(ide, i);

        FunctionData *functionData = isFunction(line);
        if (functionData != NULL) {
            if (functionData->callingFunctionSignature) {
                functionCallerCase(tree, ide, line, functionData);

            } else {
                if (ideGetMyStringLineIndex(functionsNamesSaver, functionData->functionName) != -1) {
                    printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
                    destroyStackC(functionData->parametersStack);
                    destroyString(functionData->functionName);
                    free(functionData);

                } else {
                    ideAddMyStringLine(functionsNamesSaver, functionData->functionName);
                    functionCallieCase(ide, line, functionData, &i, functionsStack);
                }
            }
        }

        else if (isFunctionEnd(line)) {
            if (stackIsEmpty(functionsStack))
                printf(ANSI_COLOR_BOLD_RED "SYNTAX ERROR: %s\n" ANSI_COLOR_RESET, line);
            else
            stackPop(functionsStack);

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

    VariablesVector *variablesVector = avlTreeToVariablesVector(tree);
    VariableBox *variablesArray = variablesVectorToArray(variablesVector);
    int variablesArrayLength = variablesVectorLength(variablesVector);
    destroyVariablesVector(variablesVector);

    buildmaxheap(variablesArray, variablesArrayLength);

    printingVariablesChoices(tree, variablesArray, variablesArrayLength);

    for (int i = 0; i < variablesArrayLength; i++)
        free(variablesArray[i].key);

    free(variablesArray);
    destroyStack(ifConditionsStack);
    destroyStack(functionsStack);
    destroyStack(loopsStack);
    ideDestroy(functionsNamesSaver);
    avlTreeDestroy(tree);

}

void printingVariablesChoices(AVLTree *tree, VariableBox *heapArray, int heapArrayLength) {
    char choice[3];
    while (1) {
        printf(ANSI_COLOR_BOLD_CYAN "\nPlease choose from the following:\n"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA "1- Print variables sorted by key weight.\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA "2- Print variables sorted by value.\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA "3- Return back.\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_BOLD_MAGENTA " :- " ANSI_COLOR_RESET);
        fgets(choice, 3, stdin);
        if (choice[strlen(choice) - 1] == '\n')
            choice[strlen(choice) - 1] = '\0';

        if (!strcmp(choice, "1"))
            inOrderTraversal(tree->root);
        else if (!strcmp(choice, "2"))
            displayHeap(heapArray, heapArrayLength);
        else if (!strcmp(choice, "3"))
            break;
        else
            printf(ANSI_COLOR_BOLD_RED "Please enter a valid choice.\n" ANSI_COLOR_RESET);

        printf(ANSI_COLOR_BOLD_MAGENTA "Click Enter to continue.\n\n" ANSI_COLOR_RESET);
        fseek(stdin, 0, SEEK_END);
        fgetc(stdin);
        fseek(stdin, 0, SEEK_END);
    }
}