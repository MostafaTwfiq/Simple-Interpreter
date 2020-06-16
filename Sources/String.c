#include "../Headers/String.h"

String *stringInitialization(int initialLength) {
    String *myString = (String *) malloc(sizeof(String));
    myString->string = (char *) malloc(sizeof(char) * (initialLength + 1) );
    myString->string[0] = '\0';
    myString->count = 0;
    myString->length = initialLength;

    return myString;
}

void stringAddCharAtLast(String *myString, char c) {
    if (myString == NULL)
        exit(-002);

    if (myString->count == myString->length) {
        myString->length *= 2;
        myString->string = realloc(myString->string, sizeof(char) * (myString->length + 1));
    }

    myString->string[myString->count++] = c;
    myString->string[myString->count] = '\0';
}

void stringAddCharAtIndex(String *myString, int index, char c) {
    if (myString == NULL || myString->count <= index || index < 0)
        exit(-002);

    myString->string[index] = c;
}

void stringRemoveCharAtIndex(String *myString, int index) {
    if (myString == NULL || myString->count <= index || index < 0)
        exit(-001);

    for (int i = index; i < myString->count; i++)
        myString->string[i] = myString->string[i + 1];

    myString->count--;
}

void stringAddStringAtLast(String *myString, char *newString) {
    if (myString == NULL || newString == NULL)
        exit(-002);

    int newStringLength = strlen(newString);
    if (newStringLength > myString->length - myString->count) {
        myString->length = ( myString->length + ( newStringLength - (myString->length - myString->count) ) ) * 2;
        myString->string = realloc(myString->string, sizeof(char) * (myString->length + 1));
    }

    for (int i = 0; i < newStringLength; i++)
        myString->string[myString->count++] = newString[i];

    myString->string[myString->count] = '\0';

}

void stringChangeStringByCharArray(String *string, char *newString) {
    if (string == NULL || newString == NULL)
        exit(-002);

    string->count = 0;

    while (*newString != '\0')
        stringAddCharAtLast(string, *newString++);

}

void stringChangeStringByString(String *string, String *newString) {
    if (string == NULL || newString == NULL)
        exit(-002);

    string->count = 0;
    string->string[string->count] = '\0';

    for (int i = 0; i < stringGetLength(newString); i++)
        stringAddCharAtLast(string, stringGetCharAtIndex(newString, i));


}

int stringFindCharIndex(String *myString, char c) {
    if (myString == NULL)
        exit(-002);

    for (int i = 0; i < myString->count; i++) {
        if (myString->string[i] == c)
            return i;

    }

    return -1;
}

char stringGetCharAtIndex(String *myString, int index) {
    if (myString == NULL || myString->count <= index || index < 0)
        exit(-002);

    return myString->string[index];
}

int stringSubString(String *myString, char *string) {
    if (myString == NULL || string == NULL)
        exit(-002);

    int stringLength = strlen(string);

    if (string == NULL || stringLength > myString->count)
        return 0;

    for (int i = 0; i <= myString->count - stringLength; i++) {
        for (int j = 0; j < stringLength; j++) {
            if (myString->string[i + j] != string[j])
                break;

            else if (j == stringLength - 1)
                return 1;

        }
    }

    return 0;
}

char *stringToArrayOfCharacters(String *myString) {
    if (myString == NULL)
        exit(-002);

    char *arrayOfCharacters = (char *) malloc(sizeof(char) * (myString->count + 1) );

    for (int i = 0; i <= myString->count; i++)
        arrayOfCharacters[i] = myString->string[i];

    return arrayOfCharacters;
}

char *stringGetStringBetweenTwoIndices(String *myString, int startIndex, int endIndex) {
    if (myString == NULL)
        exit(-002);

    if (startIndex > endIndex
        || startIndex < 0 || startIndex >= myString->count
        || endIndex < 0 || endIndex >= myString->count) {

        return NULL;
    }

    char *string = (char *) malloc(sizeof(char) * (endIndex - startIndex + 2) );
    string[endIndex - startIndex + 1] = '\0';
    for (int i = 0; i <= endIndex - startIndex; i++)
        string[i] = myString->string[startIndex + i];

    return string;

}

int stringGetLength(String *myString) {
    if (myString == NULL)
        exit(-002);

    return myString->count;
}

int stringIsEqualsToCharArray(String *myString, char *string) {
    if (string == NULL)
        return 0;

    int stringLength = strlen(string);
    if (myString->count != stringLength)
        return 0;

    for (int i = 0; i < myString->count; i++) {

        if (myString->string[i] != string[i])
            return 0;

    }

    return 1;
}

int stringIsEqualsToMyString(String *myString, String *string) {
    if (myString == NULL || string == NULL)
        exit(-002);

    int stringLength = stringGetLength(string);
    if (string == NULL || myString->count != stringLength)
        return 0;

    for (int i = 0; i < stringLength; i++) {

        if (myString->string[i] != string->string[i])
            return 0;

    }

    return 1;
}

void stringTrim(String *myString) {
    if (myString == NULL)
        return;

    while (1) {
        if (*myString->string == ' ') {

            for (int i = 0; i < stringGetLength(myString); i++)
                myString->string[i] = myString->string[i + 1];

            myString->count--;

        }

        else
            break;

    }

    for (int i = stringGetLength(myString) - 1; i >= 0; i--) {

        if (myString->string[i] == ' ') {
            myString->string[i] = '\0';
            myString->count--;
        }
        else
            break;

    }
}

void stringFGetS(String *myString) {
    if (myString == NULL)
        exit(-002);


    char c;

    while (1) {
        c = fgetc(stdin);

        if (c == '\n')
            break;

        stringAddCharAtLast(myString, c);
    }

    fseek(stdin, 0, SEEK_END);
}

void destroyString(String *myString) {
    if (myString == NULL)
        exit(-002);

    free(myString->string);
    free(myString);
}