# ifndef HE_H
#define HE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_types {cmd, pip, red, file} t_types;

typedef struct  s_str_part
{
	char	*str;
	t_types	type;
	short	level;
}	t_str_part;

// typedef struct  s_tree
// {
// 	char	*right;
// 	char	*left;
// }	t_tree;
typedef struct s_cmd {
    char **args;
    char *input;
    char *output;
    int append;
    char *heredoc;
    struct s_cmd *next;
} t_cmd;

int	ft_strncmp(const char *s1, const char *s2, size_t n);

//split
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);

#endif