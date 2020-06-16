#include "../../Headers/Compiler/CommentsHandling.h"

int isComment(char *line) {
    if (strlen(line) >= 2 && line[0] == '/' && line[1] == '/')
        return 1;

    return 0;
}

void commentCase(char *line) {
    printf(ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, line);
}