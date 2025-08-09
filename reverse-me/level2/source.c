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
    char input[64];    // Buffer pour scanf
    char key[10];      // Buffer reconstruit (caractères convertis)
    int i = 2;         // On commence à lire après les deux premiers caractères
    int j = 1;         // L'index du buffer `key`
    int scanf_result;

    printf("Please enter key: ");
    scanf_result = scanf("%s", input);   // prend la chaîne entrée

    if (scanf_result != 1)
        no();

    if (input[0] != '0' || input[1] != '0')
        no();

    fflush(stdout);  // s'assure que tout est affiché

    memset(key, 0, 9);  // remet key à 0
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
    key[j] = '\0';  // fin de chaîne
    // comparaison avec mot de passe attendu (dans le .rodata)
    if (strcmp(key, "delaber") == 0)
        ok();
    else
        no();

    return 0;
}
