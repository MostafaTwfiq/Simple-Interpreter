#include "../../Headers/Compiler/InfixCalculator.h"

///Case one:
///if the operator stack is empty or the current operator has a higher priority from the operator stack stackCPeek,
/// then stackCPush the current operator directly to the operator stack
///Special Case: if the operator is a right associative and the priority of the stackCPeek of the stack is smaller ot the same or ,
/// then stackCPush the operator directly to the operator stack

///Case two:
///if the current operator has a smaller or equal priority from the stackCPeek of the operator stack,
///// then stackCPop every element from the operator stack until the stack is empty or the stackCPeek has a smaller priority,
/// then stackCPush the current operator to the operator stack.

///Case three:
///if the current operator is << + or - >> and the ( length of the operator stack - number of open parenthesis - number of not gates == the length of the postfix stack )
/// then push a multiply sign to the operator stack and multiply 1 in the current operator then stackCPush the multiply answer
/// to the postfix stack

//List of operators:
//-----------------
// !                                           P13
// **                                          P12
// * / %                                       P11
// + -                                         P10
// << >>                                       P9
// < <= > >=                                   P8
// == !=                                       P7
// &                                           P6
// ^                                           P5
// |                                           P4
// &&                                          P3
// ||                                          P2
// = *= /= %= += -= <<= >>= &= |= ^=           P1

typedef enum operator {
    EQUAL = 1, NOTEQUAL, POWER,
    AND, OR, NOT,
    BITWISE_AND, XOR, BITWISE_OR,
    MULTIPLICATION, DIVISION,
    MOD, PLUS, MINUS, ASSIGN,
    GREATER_THAN, GREATER_THAN_OR_EQUAL,
    SMALLER_THAN, SMALLER_THAN_OR_EQUAL,
    LeftShift, RightShift,
    MULTIPLY_ASSIGN, DIVISION_ASSIGN, MOD_ASSIGN,
    PLUS_ASSIGN, MINUS_ASSIGN,
    BITWISE_AND_ASSIGN, BITWISE_OR_ASSIGN, XOR_ASSIGN,
    LEFT_SHIFT_ASSIGN, RIGHT_SHIFT_ASSIGN
} operator;

float *calculateInfix(AVLTree *symbolTable, char *infix) {
    StackC *operatorStack = stackCInitialization();
    StackC *postfixStack = stackCInitialization();
    int openParenthesisCount = 0, notSignCount = 0, errorFlag = 0;

    for (int i = 0; i < strlen(infix); i++) {
        if (isAVariableCharacter(infix + i)) {
            variableCase(postfixStack, infix + i, &i);
        }

        else if (isOperator(infix + i)) {
            operatorCase(symbolTable, operatorStack, postfixStack, infix + i, openParenthesisCount, &notSignCount, &errorFlag);
            i += getOperatorLength(infix + i) - 1;
        }

        // x = 2 + 5 (+5)
        else if (infix[i] == '(') {

            while (infix[i + 1] == ' ') {
                i++;
            }

                if (infix[i + 1] != ')' && ( infix[i + 1] == '+' || infix[i + 1] == '(' || infix[i + 1] == '-' || infix[i + 1] == '!' || isAVariableCharacter(infix + i + 1)) ) {

                    if (infix[i + 1] == '+' || infix[i + 1] == '-') {
                        if (getCaseNumber(operatorStack, postfixStack, infix + i + 1, openParenthesisCount, notSignCount) != 3) {
                            errorFlag = 1;
                        }

                    }

                    char *parenthesisChar = (char *) malloc(sizeof(char) * 2);
                    parenthesisChar[1] = '\0';
                    parenthesisChar[0] = '(';
                    stackCPush(operatorStack, parenthesisChar);
                    openParenthesisCount++;
                } else
                    errorFlag = 1;

        }

        else if (infix[i] == ')') {
            if ( !stackCIsEmpty(operatorStack)
                 && (stackCGetLength(operatorStack) - openParenthesisCount - notSignCount == stackCGetLength(postfixStack) - 1)) {

                while (*stackCPeek(operatorStack) != '(') {
                    evaluate(symbolTable, postfixStack, stackCPop(operatorStack), &notSignCount, &errorFlag);

                    if (stackCIsEmpty(operatorStack)) {
                        errorFlag = 1;
                        break;
                    }

                }
                if (!errorFlag) {
                    free(stackCPop(operatorStack));
                    openParenthesisCount--;
                }

            } else
                errorFlag = 1;

        }

        else if (infix[i] == ' ')
            continue;

        else {
            destroyStackC(operatorStack);
            destroyStackC(postfixStack);
            return NULL;
        }

        if (errorFlag) {
            destroyStackC(operatorStack);
            destroyStackC(postfixStack);
            return NULL;
        }

    }

    float *finalAns = evaluateTheRestOfThePostfix(symbolTable, operatorStack, postfixStack, &notSignCount);
    destroyStackC(operatorStack);
    destroyStackC(postfixStack);
    return finalAns;

}
//evaluates what is left of the postfix stack
float *evaluateTheRestOfThePostfix(AVLTree *symbolTable, StackC *operatorStack, StackC *postfixStack, int *notSignCount) {
    if (stackCIsEmpty(operatorStack) && stackCIsEmpty(postfixStack))
        return NULL;

    if (stackCGetLength(operatorStack) - *notSignCount != stackCGetLength(postfixStack) - 1) {
        return NULL;
    }

    int errorFlag = 0;
    while (!stackCIsEmpty(operatorStack)) {
        evaluate(symbolTable, postfixStack, stackCPop(operatorStack), notSignCount, &errorFlag);

        if (errorFlag)
            return NULL;

    }

    float *finalAns = getVariableValue(symbolTable, stackCPop(postfixStack));

    return finalAns;
}

int isRightAssociative(char *c) {
    return (getOperatorLength(c) == 2 && !strncmp(c, "**", 2))
           || isAssignOperator(c)
           || !strncmp(c, "!", getOperatorLength(c));
}


int getCaseNumber(StackC *operatorStack   ,  StackC *postfixStack, char *infix,
                  int openParenthesisCount,  int notSignCount) {
    //Case 3
    // unary plus or minus
    if ( (!strncmp(infix, "+", 1) || !strncmp(infix, "-", 1))
         && stackCGetLength(operatorStack) - openParenthesisCount - notSignCount == stackCGetLength(postfixStack)) {

        return 3;
    }
        // Case 1
        // if the operator is right associative.
    else if ( isRightAssociative(infix)
              && ( stackCIsEmpty(operatorStack)
                   || getPriority(infix) >= getPriority(stackCPeek(operatorStack)))) {
        return 1;
    }
        //Case 1
        // if the stack is empty then push onto the stack or the priority of the current operator is larger
    else if (stackCIsEmpty(operatorStack)
             || getPriority(infix) > getPriority(stackCPeek(operatorStack))) {

        return 1;
    }
        // Case 2
        // if the priority of the operator is lower or equal to that which is on top the stack
    else if (getPriority(infix) <= getPriority(stackCPeek(operatorStack))) {
        return 2;
    }
    return 0;

}

int isIntegerNumber(char *number) {
    int dotCounter = 0;
    for (int i = 0; i < strlen(number); i++) {
        if (number[i] == '.')
            dotCounter++;

    }

    if (dotCounter > 1)
        return 0;

    while (*number != '.' && *number != '\0')
        number++;

    for (int i = 1; i < strlen(number); i++) {
        if (number[i] != '0')
            return 0;

    }

    return 1;
}

int isAssignOperator(char *c) {
    int length = strlen(c);

    if (length > 2 && (!strncmp(c, "<<=", 3) || !strncmp(c, ">>=", 3)))
        return 1;

    else if (length > 1 && (
            !strncmp(c, "*=", 2)        || !strncmp(c, "/=", 2)
            ||!strncmp(c, "%=", 2)      || !strncmp(c, "+=", 2)
            ||!strncmp(c, "-=", 2)      || !strncmp(c, "&=", 2)
            ||!strncmp(c, "|=", 2)      || !strncmp(c, "^=", 2))) {

        return 1;
    }

    else if (length > 0 && ( !strncmp(c, "=", 1)))
        return 1;

    return 0;

}

void evaluate(AVLTree *symbolTable, StackC *postfixStack, char *infix, int *notSignCount, int *errorFlag){
    int operatorLength = getOperatorLength(infix);
    char *operator = (char *) malloc(sizeof(char) * (operatorLength + 1) );
    operator[operatorLength] = '\0';
    strncpy(operator, infix, operatorLength);
    if(isAssignOperator(operator)) {
        assignVariable(symbolTable, postfixStack, operator, errorFlag);
    }

    else if (!strcmp(operator, "!")) {
        notOperatorCase(symbolTable, postfixStack, notSignCount, errorFlag);
    }

    else {
        remainingOperatorsEvaluate(symbolTable, postfixStack, operator, errorFlag);
    }

    free(operator);
    free(infix);
}

void operatorCase(AVLTree *symbolTable, StackC *operatorStack, StackC *postfixStack, char *infix,
                  int openParenthesisCount, int *notSignCount, int *errorFlag) {

    int operationCase  =  getCaseNumber(operatorStack, postfixStack, infix, openParenthesisCount,  *notSignCount);
    if (operationCase  == 1 ){
        int operatorLength  = getOperatorLength(infix);
        char * operator =  (char *) malloc(sizeof(char ) * ( operatorLength + 1 ) );
        operator[operatorLength] = '\0';
        strncpy(operator, infix, operatorLength);
        stackCPush(operatorStack, operator);
    }

    else if(operationCase  == 2 ) {
        if (stackCGetLength(operatorStack) - openParenthesisCount - *notSignCount != stackCGetLength(postfixStack) - 1) {
            *errorFlag = 1;
            return;
        }

        while (!stackCIsEmpty(operatorStack) && getPriority(stackCPeek(operatorStack)) >= getPriority(infix)) {
            evaluate(symbolTable, postfixStack, stackCPop(operatorStack), notSignCount, errorFlag);

            if (*errorFlag) {
                return;
            }

        }

        int operatorLength = getOperatorLength(infix);
        char *operator = (char *) malloc(sizeof(char) * (operatorLength + 1) );
        operator[operatorLength] = '\0';
        strncpy(operator, infix, operatorLength);
        stackCPush(operatorStack, operator);

    }

    else if(operationCase  == 3 ){
        char *operator =  (char *) malloc(sizeof(char ) *  2 );
        operator[0] = '*';
        operator[1] ='\0';
        stackCPush(operatorStack, operator);
        float val = *infix == '-' ? -1.0 : 1.0;
        char *valueString = (char *) malloc(sizeof(char) * 60);
        valueString[59] ='\0';
        sprintf(valueString,"%.2f",val);
        stackCPush(postfixStack,valueString);
    }

    else
        *errorFlag = 1;

    if (!strncmp(infix, "!", getOperatorLength(infix))) {
        *notSignCount += 1;

        if (stackCGetLength(operatorStack) - openParenthesisCount - *notSignCount != stackCGetLength(postfixStack))
            *errorFlag = 1;

    }

}

/**this function will be called when the current operator is an assign operator, so it will pop the variable from the postfixStack
    and it's value then it will assign the value to it, then it will be added to the tree  */
void assignVariable(AVLTree *symbolTable, StackC *postfixStack, char *operator, int *errorFlag){
    if (stackCGetLength(postfixStack) < 2) {
        *errorFlag = 1;
        return;
    }

    char *value    = stackCPop(postfixStack);
    char *varName  = stackCPop(postfixStack);
    if (!isLegalVariableName(varName)) {
        free(value);
        free(varName);
        *errorFlag = 1;
        return;
    }

    float *floatValue = getVariableValue(symbolTable, value);
    float finalValue = 0;

    if (floatValue == NULL) {
        *errorFlag = 1;
        free(value);
        free(varName);
        return;
    }

    int operatorNumber = isOperator(operator);

    if (operatorNumber == ASSIGN) {
        finalValue = *floatValue;
    }

    else if (operatorNumber == MULTIPLY_ASSIGN || operatorNumber == DIVISION_ASSIGN
            || operatorNumber == PLUS_ASSIGN || operatorNumber == MINUS_ASSIGN) {

        NodeValue variableNode = BSGetNodeValue(symbolTable->root, varName);
        if (*variableNode.type == '\0') {
            *errorFlag = 1;
            free(varName);
            free(value);
            free(floatValue);
            return;
        }

        if (operatorNumber == MULTIPLY_ASSIGN)
            finalValue = variableNode.data * (*floatValue);
        else if (operatorNumber == DIVISION_ASSIGN)
            finalValue = variableNode.data / (*floatValue);
        else if (operatorNumber == PLUS_ASSIGN)
            finalValue = variableNode.data + (*floatValue);
        else
            finalValue = variableNode.data - (*floatValue);

    }

    else if (operatorNumber == MOD_ASSIGN           || operatorNumber == LEFT_SHIFT_ASSIGN
            ||operatorNumber == RIGHT_SHIFT_ASSIGN  || operatorNumber == BITWISE_AND_ASSIGN
            ||operatorNumber == BITWISE_OR_ASSIGN   || operatorNumber == XOR_ASSIGN) {

        NodeValue variableNode = BSGetNodeValue(symbolTable->root, varName);
        if (*variableNode.type == '\0') {
            *errorFlag = 1;
            free(varName);
            free(value);
            free(floatValue);
            return;
        }
        char *variableData = (char *) malloc(sizeof(char) * 60);
        *variableData = '\0';
        sprintf(variableData, "%f", variableNode.data);

        if (!isIntegerNumber((char *) floatValue) || !isIntegerNumber(variableData)) {
            *errorFlag = 1;
            free(varName);
            free(value);
            free(floatValue);
            free(variableData);
            return;
        }

        if (operatorNumber == LEFT_SHIFT_ASSIGN)
            finalValue = (float) ( (int) variableNode.data << (int) (*floatValue) );
        else if (operatorNumber == RIGHT_SHIFT_ASSIGN)
            finalValue = (float) ( (int) variableNode.data >> (int) (*floatValue) );
        else if (operatorNumber == MOD_ASSIGN)
            finalValue = (float) ( (int) variableNode.data % (int) (*floatValue) );
        else if (operatorNumber == BITWISE_AND_ASSIGN)
            finalValue = (float) ( (int) variableNode.data & (int) (*floatValue) );
        else if (operatorNumber == BITWISE_OR_ASSIGN)
            finalValue = (float) ( (int) variableNode.data | (int) (*floatValue) );
        else
            finalValue = (float) ( (int) variableNode.data ^ (int) (*floatValue) );

        free(variableData);
    }

    char *finalValueCharArray = (char *) malloc(sizeof(char) * 60);
    *finalValueCharArray = '\0';
    sprintf(finalValueCharArray, "%f", finalValue);

    char *type = isIntegerNumber(finalValueCharArray) ? "int" : "float";
    NodeValue valueNode;
    valueNode.data = finalValue;
    strcpy(valueNode.type, type);
    symbolTable->root = insert(symbolTable->root, varName, valueNode);

    stackCPush(postfixStack, finalValueCharArray);
    free(varName);
    free(value);
    free(floatValue);

}

int isNumber(char *string) {
    int dotCounter = 0;
    for (int i = 0; i < strlen(string); i++) {

        if (string[i] == '.')
            dotCounter++;
        else if ( !( string[i] == '+' || string[i] == '-' || (string[i] >= '0' && string[i] <= '9') ) )
            return 0;

    }

    return dotCounter > 1 ? 0 : 1;

}

int isLegalVariableNameCharacter(char *c) {
    return (*c >= '0' && *c <= '9') || (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c == '_');
}

int isLegalVariableName(char *variable) {
    if (*variable >= '0' && *variable <= '9')
        return 0;

    for (int i = 1; i < strlen(variable); i++) {
        if (!isLegalVariableNameCharacter(variable + i))
            return 0;

    }

    return 1;
}

float *getVariableValue(AVLTree *symbolTable, char *variable) {
    float *floatValue = (float *) malloc(sizeof(float));
    if (!isNumber(variable)) {
        if (!isLegalVariableName(variable)) {
            free(floatValue);
            return NULL;
        }

        NodeValue nodeValue = BSGetNodeValue(symbolTable->root, variable);
        if (*nodeValue.type == '\0') {
            free(floatValue);
            return NULL;
        }

        *floatValue = nodeValue.data;

    } else
        *floatValue = atof(variable);

    return floatValue;
}

void notOperatorCase(AVLTree *symbolTable, StackC *postfixStack, int *notSignCount, int *errorFlag) {
    if (stackCGetLength(postfixStack) < 1) {
        *errorFlag = 1;
        return;
    }

    char *variable = stackCPop(postfixStack);
    float *floatValue = getVariableValue(symbolTable, variable);

    if (floatValue == NULL) {
        free(variable);
        *errorFlag = 1;
        return;
    }

    float finalAns = (float) !(*floatValue);
    char *finalAnsPointer = (char *) malloc(sizeof(char) * 60);
    finalAnsPointer[59] = '\0';
    sprintf(finalAnsPointer, "%.2f", finalAns);
    stackCPush(postfixStack, finalAnsPointer);

    *notSignCount -= 1;
    free(floatValue);
}

void remainingOperatorsEvaluate(AVLTree *symbolTable, StackC *postfixStack, char *operator, int *errorFlag) {
    if (stackCGetLength(postfixStack) < 2) {
        *errorFlag = 1;
        return;
    }

    char *secondVariable = stackCPop(postfixStack);
    char *firstVariable = stackCPop(postfixStack);
    float *secondFloat, *firstFloat;

    firstFloat = getVariableValue(symbolTable, firstVariable);
    secondFloat = getVariableValue(symbolTable, secondVariable);
    if (!firstFloat || !secondFloat) {
        *errorFlag = 1;
        free(firstVariable);
        free(secondVariable);
        if (firstFloat != NULL)
            free(firstFloat);
        if (secondFloat != NULL)
            free(secondFloat);

        return;
    }

    int operatorNumber = isOperator(operator);
    float answer;
    switch (operatorNumber) {
        case        EQUAL:
                    answer = (float) (*firstFloat == *secondFloat);
                    break;
        case        NOTEQUAL:
                    answer = (float) (*firstFloat != *secondFloat);
                    break;
        case        POWER:
                    answer = (float) pow(*firstFloat, *secondFloat);
                    break;
        case        AND:
                    answer = (float) (*firstFloat && *secondFloat);
                    break;
        case        OR:
                    answer = (float) (*firstFloat || *secondFloat);
                    break;
        case        GREATER_THAN:
                    answer = (float) (*firstFloat > *secondFloat);
                    break;
        case        GREATER_THAN_OR_EQUAL:
                    answer = (float) (*firstFloat >= *secondFloat);
                    break;
        case        SMALLER_THAN:
                    answer = (float) (*firstFloat < *secondFloat);
                    break;
        case        SMALLER_THAN_OR_EQUAL:
                    answer = (float) (*firstFloat <= *secondFloat);
                    break;
        case        LeftShift:
                    if (!isIntegerNumber(firstVariable) || !isIntegerNumber(secondVariable)) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) ( ((int) *firstFloat) << ((int) *secondFloat));
                    break;
        case        RightShift:
                    if (!isIntegerNumber(firstVariable) || !isIntegerNumber(secondVariable)) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) ( ((int) *firstFloat) >> ((int) *secondFloat));
                    break;
        case        BITWISE_AND:
                    if (!isIntegerNumber(firstVariable) || !isIntegerNumber(secondVariable)) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) ( ((int) *firstFloat) & ((int) *secondFloat));
                    break;
        case        XOR:
                    if (!isIntegerNumber(firstVariable) || !isIntegerNumber(secondVariable)) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) ( ((int) *firstFloat) ^ ((int) *secondFloat));
                    break;
        case        BITWISE_OR:
                    if (!isIntegerNumber(firstVariable) || !isIntegerNumber(secondVariable)) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) ( ((int) *firstFloat) | ((int) *secondFloat));
                    break;
        case        MULTIPLICATION:
                    answer = (float) (*firstFloat * *secondFloat);
                    break;
        case        DIVISION:
                    if (*secondFloat == 0) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) (*firstFloat / *secondFloat);
                    break;
        case        MOD:
                    if (!isIntegerNumber(firstVariable) || !isIntegerNumber(secondVariable)) {
                        *errorFlag = 1;
                        free(firstVariable);
                        free(secondVariable);
                        free(firstFloat);
                        free(secondFloat);
                        return;
                    }
                    answer = (float) ( ((int) *firstFloat) % ((int) *secondFloat));
                    break;
        case        PLUS:
                    answer = (float) (*firstFloat + *secondFloat);
                    break;
        case        MINUS:
                    answer = (float) (*firstFloat - *secondFloat);
                    break;
    }

    char *finalAnsPointer = (char *) malloc(sizeof(char) * 60);
    finalAnsPointer[59] = '\0';
    sprintf(finalAnsPointer, "%.2f", answer);
    stackCPush(postfixStack, finalAnsPointer);
    free(firstVariable);
    free(secondVariable);
    free(firstFloat);
    free(secondFloat);

}

void variableCase(StackC *postfixStack, char *infix, int *index) {
    int varLength = 0;
    char *varName;
    while( isAVariableCharacter(infix + varLength) ) varLength++;
    varName = (char *) malloc( sizeof( char ) * (varLength + 1) );
    strncpy(varName, infix, varLength);
    varName[varLength] = '\0';
    stackCPush(postfixStack, varName);
    *index += varLength - 1;
}

int isAVariableCharacter(char *c) {
    return (*c >= '0' && *c <= '9') || (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c == '_') || (*c == '.');
}

int isOperator(char *c){
    int length = strlen(c);
    if (c == NULL || length == 0)
        return 0;

    if (length > 2 && !strncmp(c, "<<=",3))        return LEFT_SHIFT_ASSIGN;
    if (length > 2 && !strncmp(c,">>=",3))         return RIGHT_SHIFT_ASSIGN;

    if (length > 1 && !strncmp(c, "*=",2))        return MULTIPLY_ASSIGN;
    if (length > 1 && !strncmp(c,"/=",2))         return DIVISION_ASSIGN;
    if (length > 1 && !strncmp(c,"%=",2))         return MOD_ASSIGN;
    if (length > 1 && !strncmp(c, "+=",2))        return PLUS_ASSIGN;
    if (length > 1 && !strncmp(c, "-=",2))        return MINUS_ASSIGN;
    if (length > 1 && !strncmp(c, "&=", 2))       return BITWISE_AND_ASSIGN;
    if (length > 1 && !strncmp(c, "|=",2))        return BITWISE_OR_ASSIGN;
    if (length > 1 && !strncmp(c, "^=", 2))       return XOR_ASSIGN;
    if (length > 1 && !strncmp(c, "==",2))        return EQUAL;
    if (length > 1 && !strncmp(c,"!=",2))         return NOTEQUAL;
    if (length > 1 && !strncmp(c, "**",2))        return POWER;
    if (length > 1 && !strncmp(c, "&&",2))        return AND;
    if (length > 1 && !strncmp(c, "||", 2))       return OR;
    if (length > 1 && !strncmp(c,">=",2))         return GREATER_THAN_OR_EQUAL;
    if (length > 1 && !strncmp(c, "<=",2))        return SMALLER_THAN_OR_EQUAL;
    if (length > 1 && !strncmp(c,"<<",2))         return LeftShift;
    if (length > 1 && !strncmp(c, ">>",2))        return RightShift;

    if (!strncmp(c, ">",1))         return GREATER_THAN;
    if (!strncmp(c, "<",1))         return SMALLER_THAN;
    if (!strncmp(c, "!" ,1))        return NOT;
    if (!strncmp(c, "&" ,1))        return BITWISE_AND;
    if (!strncmp(c, "^",1))         return XOR;
    if (!strncmp(c, "|",1))         return BITWISE_OR;
    if (!strncmp(c,"*" ,1))         return MULTIPLICATION;
    if (!strncmp(c,"/" ,1))         return DIVISION;
    if (!strncmp(c,"%",1))          return MOD;
    if (!strncmp(c, "+",1))         return PLUS;
    if (!strncmp(c,"-",1))          return MINUS;
    if (!strncmp(c,"=",1))          return ASSIGN;
    return 0;
}

int getPriority(char *c) {
    if (c == NULL)
        return 0;

    int length = strlen(c);

    if (length > 2 && !strncmp(c, "<<=",3))        return 1;
    if (length > 2 && !strncmp(c,">>=",3))         return 1;

    if (length > 1 && !strncmp(c, "*=",2))        return 1;
    if (length > 1 && !strncmp(c,"/=",2))         return 1;
    if (length > 1 && !strncmp(c,"%=",2))         return 1;
    if (length > 1 && !strncmp(c, "+=",2))        return 1;
    if (length > 1 && !strncmp(c, "-=",2))        return 1;
    if (length > 1 && !strncmp(c, "&=", 2))       return 1;
    if (length > 1 && !strncmp(c, "|=",2))        return 1;
    if (length > 1 && !strncmp(c, "^=", 2))       return 1;
    if (length > 1 && !strncmp(c, "==",2))    return 7;
    if (length > 1 && !strncmp(c,"!=",2))     return 7;
    if (length > 1 && !strncmp(c, "**",2))    return 12;
    if (length > 1 && !strncmp(c,"<<",2))     return 9;
    if (length > 1 && !strncmp(c, ">>",2))    return 9;
    if (length > 1 && !strncmp(c, "&&",2))    return 3;
    if (length > 1 && !strncmp(c, "||", 2))   return 2;
    if (length > 1 && !strncmp(c,">=",2))     return 8;
    if (length > 1 && !strncmp(c, "<=",2))    return 8;

    if (!strncmp(c, ">",1))     return 8;
    if (!strncmp(c, "<",1))     return 8;
    if (!strncmp(c, "!" ,1))    return 13;
    if (!strncmp(c, "&" ,1))    return 6;
    if (!strncmp(c, "^",1))     return 5;
    if (!strncmp(c, "|",1))     return 4;
    if (!strncmp(c,"*" ,1))     return 11;
    if (!strncmp(c,"/" ,1))     return 11;
    if (!strncmp(c,"%",1))      return 11;
    if (!strncmp(c, "+",1))     return 10;
    if (!strncmp(c,"-",1))      return 10;
    if (!strncmp(c,"=",1))      return 1;

    return 0;
}

int getOperatorLength(char *c){
    int length = strlen(c);

    if (length < 1) {
        return 0;
    }

    if (length > 2 && (
            !strncmp(c, "<<=", 3)   ||!strncmp(c, ">>=", 3) )) {

        return 3;
    }

    if (length > 1 && (
            !strncmp(c, "==",2)         ||!strncmp(c,"!=",2)    ||!strncmp(c, "**",2)
            ||!strncmp(c, "&&",2)       ||!strncmp(c, "||", 2)  ||!strncmp(c, ">=", 2)
            ||!strncmp(c, "<=", 2)      ||!strncmp(c, "<<", 2)  ||!strncmp(c, ">>", 2)
            ||!strncmp(c, "*=", 2)      ||!strncmp(c, "/=", 2)  ||!strncmp(c, "%=", 2)
            ||!strncmp(c, "+=", 2)      ||!strncmp(c, "-=", 2)  ||!strncmp(c, "&=", 2)
            ||!strncmp(c, "|=", 2)      ||!strncmp(c, "^=", 2) )){

        return 2;
    }

    if(!strncmp(c, "!" ,1)      ||!strncmp(c, "&" ,1)
       ||!strncmp(c, "^",1)     ||!strncmp(c, "|",1)
       ||!strncmp(c,"*" ,1)     ||!strncmp(c,"/" ,1)
       ||!strncmp(c, "+",1)     ||!strncmp(c,"-",1)
       ||!strncmp(c,"%",1)      ||!strncmp(c,"=",1)
       ||!strncmp(c, ">", 1)    ||!strncmp(c, "<", 1) ){

        return 1;
    }

}