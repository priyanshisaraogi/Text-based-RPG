#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void wait_for_enter(void) {
    int c;
    while ((c = getchar()) != '\n') {
        /* Consume extra characters */
    }
}

void print_pause(const char *line) {
    printf("\n%s\n", line);
    wait_for_enter();
}

int get_int_input(const char *prompt) {
    int choice;
    char input[128];
    char *endptr;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            choice = strtol(input, &endptr, 10);
            if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
                printf("Wrong input, please choose a numeric value.\n");
                continue;
            }
            return choice;
        } else {
            printf("Input error, please try again.\n");
        }
    }
}
