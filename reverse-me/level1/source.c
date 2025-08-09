#include <stdio.h>
#include <string.h>

int main() {
    char password[] = "__stack_check";
    char input[100];

    printf("Please enter key: ");
    scanf("%99s", input);

    if (strcmp(input, password) == 0) {
        printf("Good job.\n");
    } else {
        printf("Nope.\n");
    }

    return 0;
}
