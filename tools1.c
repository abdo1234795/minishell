
#include "he.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

void add_token(t_token **tokens, char *value, t_types type) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_token->value = strdup(value);
    if (!new_token->value) {
        perror("strdup");
        free(new_token);
        exit(EXIT_FAILURE);
    }
    
    new_token->type = type;
    new_token->next = NULL;

    if (*tokens == NULL) {
        *tokens = new_token;
    } else {
        t_token *current = *tokens;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_token;
    }
}

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}