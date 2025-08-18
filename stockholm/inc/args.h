#ifndef ARGS_H
#define ARGS_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

typedef struct s_args
{
    bool    _help, _version, _silent, _reverse;
    char    *_key;
} t_args;

t_args  *args_empty();

#endif