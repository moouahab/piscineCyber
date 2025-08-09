#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void    __system__malloc() {
    exit(EXIT_FAILURE);
}

int main() {
    char input[100];
    char reconstructed[9]; // 8 chars + null terminator
    int i = 0;
    int j = 2; // on saute les deux premiers caractères

    printf("Please enter key: ");
    scanf("%s", input);
    if (input[0] != '4') __system__malloc();
    if (input[1] != '2') __system__malloc();
    fflush(stdout);
    memset(reconstructed, 0, 9);
    reconstructed[0] = '*';

    while (i < 7) {
        char tmp[4] = {0};
        tmp[0] = input[j];
        tmp[1] = input[j + 1];
        tmp[2] = input[j + 2];
        reconstructed[i + 1] = (char)atoi(tmp); // index i+1 car [0] est '*'
        j += 3;
        i++;
    }
    if (strcmp(reconstructed, "********") == 0) {
        puts("Good job.");
    } else {
        puts("Nope.");
        __system__malloc();
    }

    return 0;
}
