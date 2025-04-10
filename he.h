# ifndef HE_H
#define HE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_types
{cmd, pip, red, file, text} t_types;

typedef enum e_quote_state {
    UNQUOTED,
    SINGLE_QUOTED,
    DOUBLE_QUOTED
} t_quote_state;



typedef struct s_token {
    char *value;
    t_types type;
    struct s_token *next;
} t_token;


// typedef struct  s_tree
// {
// 	char	*right;
// 	char	*left;
// }	t_tree;




int	ft_strcmp(const char *s1, const char *s2);

//split
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);

int ft_isspace(char c);

#endif