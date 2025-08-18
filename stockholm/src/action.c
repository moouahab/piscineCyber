#include <stdio.h>

void print_help(const char *progname)
{
    printf("Usage: %s [OPTIONS]\n", progname);
    printf("\nOptions disponibles:\n");
    printf("  -h, --help            Affiche cette aide et quitte\n");
    printf("  -v, --version         Affiche la version du programme\n");
    printf("  -s, --silent          Mode silencieux (aucun affichage pendant le chiffrement)\n");
    printf("  -r, --reverse <clé>   Déchiffre les fichiers en utilisant la clé fournie\n");
    printf("\nDescription:\n");
    printf("  Ce programme chiffre les fichiers du dossier ~/infection\n");
    printf("  avec un algorithme sûr, puis les renomme en ajoutant l'extension .ft.\n");
    printf("  Utilisez --reverse avec la clé correcte pour restaurer les fichiers.\n");
    printf("\n⚠ Projet éducatif uniquement. Ne jamais utiliser à des fins malveillantes.\n");
}
