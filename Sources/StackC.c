#include "../Headers/StackC.h"

StackC *stackCInitialization() {
    StackC *stack = (StackC *) malloc(sizeof(StackC));
    stack->length = 25;
    stack->count = 0;
    stack->arr = (char **) malloc(sizeof(char *) * stack->length);

    return stack;
}

void stackCPush(StackC *stack, char *string) {

    if (stack->count == stack->length) {
        stack->length *= 2;
        stack->arr = (char **) realloc(stack->arr, sizeof(char *) * stack->length);
    }

    stack->arr[stack->count++] = string;
}

char *stackCPop(StackC *stack) {
    if (stack->count == 0)
        exit(-10);

    return stack->arr[--stack->count];

}

char *stackCPeek(StackC *stack) {
    if (stack->count == 0)
        exit(-10);

    return stack->arr[stack->count - 1];
}

int stackCGetLength(StackC *stack) {
    return stack->count;
}

int stackCIsEmpty(StackC *stack) {
    return stack->count == 0;
}

void destroyStackC(StackC *stack) {
    for (int i = 0; i < stack->count; i++)
        free(stack->arr[i]);

    free(stack->arr);
    free(stack);
}