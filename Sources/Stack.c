#include "../Headers/Stack.h"

Stack *stackInitialization()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->count = 0;
    stack->length = 100;
    stack->arr = (Item *) malloc(sizeof(Item) * stack->length);
    return stack;
}

void stackPush(Stack *stack, Item value)
{
    if (stack->count == stack->length) {
        stack->length = stack->length * 2;
        stack->arr = realloc(stack->arr, sizeof(Item) * stack->length);
    }

    stack->arr[stack->count++] = value;
}

Item stackPop(Stack *stack)
{
    if (stack->count == 0)
        exit(-1);

    return stack->arr[--stack->count];
}

Item stackPeek(Stack *stack)
{
    if (stack->count == 0)
        exit(-1);
    return stack->arr[stack->count - 1];
}

int stackLength(Stack *stack) {
    return stack->count;
}

int stackIsEmpty(Stack *stack)
{
    return stack->count == 0;
}

void destroyStack(Stack *stack) {
    free(stack->arr);
    free(stack);
}