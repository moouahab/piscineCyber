#include "args.h"

extern char *parse_key(const char *s); // à toi de l’implémenter (valider longueur >= 16, etc.)

t_args *args_empty(void) {
    t_args *a = malloc(sizeof(*a));
    if (!a) return NULL;
    a->_help = a->_version = a->_silent = a->_reverse = false;
    a->_key = NULL;
    return a;
}

static int isopt(const char *arg, const char *s, const char *l) {
    return (arg && ((s && strcmp(arg, s) == 0) || (l && strcmp(arg, l) == 0)));
}

t_args *scan_args(int ac, char **av, int tail_min, int tail_max)
{
    if (ac < tail_min || ac > tail_max) {
        fprintf(stderr, "Usage invalide: nombre d'arguments hors bornes (%d..%d)\n",
                tail_min, tail_max);
        exit(EXIT_FAILURE);
    }

    t_args *args = args_empty();
    
    if (!args) {
        fprintf(stderr, "Erreur: allocation t_args\n");
        exit(EXIT_FAILURE);
    }
    // Parcours de tous les arguments optionnels
    for (int i = 1; i < ac; ++i) {
        const char *arg = av[i];

        if (isopt(arg, "-h", "--help"))
            args->_help = true;
            // On peut décider de court-circuiter ici (le main sortira après affichage)
        else if (isopt(arg, "-v", "--version"))
            args->_version = true;
        else if (isopt(arg, "-s", "--silent"))
            args->_silent = true;
        else if (isopt(arg, "-r", "--reverse")) {
            // Besoin d'une clé juste après
            if (i + 1 >= ac) {
                fprintf(stderr, "Erreur: %s nécessite une clé\n", arg);
                exit(EXIT_FAILURE);
            }
            const char *raw = av[++i]; // consomme la clé
            char *k = parse_key(raw);  // ex: vérifie longueur >= 16, renvoie NULL si invalide
            if (!k) {
                fprintf(stderr, "Erreur: clé invalide (min 16 caractères recommandés)\n");
                exit(EXIT_FAILURE);
            }
            args->_reverse = true;
            args->_key = k; // k peut être un strdup(raw) dans parse_key()
        }
        else {
            // Argument non reconnu → aide
            fprintf(stderr, "Option inconnue: %s\n", arg);
            args->_help = true;
        }
    }
    // Règles de cohérence (exemples, à adapter à ta politique)
    if (args->_reverse && !args->_key) {
        fprintf(stderr, "Erreur: mode --reverse sans clé\n");
        exit(EXIT_FAILURE);
    }
    return args;
}
