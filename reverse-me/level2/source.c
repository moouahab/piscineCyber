#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ok() {
    printf("Good job.\n");
}

void no() {
    printf("Nope.\n");
}

int main() {
    char input[64];
    char key[10];
    int i = 2;
    int j = 1;
    int scanf_result;

    printf("Please enter key: ");
    scanf_result = scanf("%s", input);

    if (scanf_result != 1)
        no();

    if (input[0] != '0' || input[1] != '0')
        no();

    fflush(stdout);  // s'assure que tout est affiché

    memset(key, 0, 9);
    key[0] = 'd';       // le premier caractère est fixé à 'd'
    key[1] = '\0';

    while (1) {
        if (strlen(input) < 8 || j >= strlen(input) / 3)
            break;

        // On lit 3 caractères à partir de input[i]
        char temp[4] = {0};
        temp[0] = input[i];
        temp[1] = input[i + 1];
        temp[2] = input[i + 2];

        int val = atoi(temp);        // conversion des 3 digits
        key[j] = (char)val;          // conversion ASCII
        j++;
        i += 3;
    }
    key[j] = '\0';
    if (strcmp(key, "delaber") == 0)
        ok();
    else
        no();

    return 0;
}
