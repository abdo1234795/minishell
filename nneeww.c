#include "he.h"

char *remove_c(char *s, char c)
{
	char	*new;
	int i =-1;
	int j = 0;

	new = malloc(ft_strlen(s) + 1);
	if(!new)
		return (NULL);
	while (s[++i])
	{
		if (s[i] != c)
			new[j++] = s[i];
	}
	new[j] = '\0';
	return (new);
}
int second_check(char *buffer, t_quote_state *state)
{
	if (state == UNQUOTED && buffer[0] == '<' && (!buffer[1] || buffer[1] == '<'))
		return(1);
	if (state == UNQUOTED && buffer[0] == '>' && (!buffer[1] || buffer[1] == '>'))
		return(1);
	if (state == UNQUOTED && buffer[0] == '|' && !buffer[1])
		return(1);
	return (0);// write syntax error !!!!!!
}


void get_type_add_token(t_token **tokens, char *buffer, t_quote_state *state)
{

	if (!second_check(buffer, state))
		return ;
	
	char *tmp = remove_c(buffer, '\"');
    if (!tmp)
		return ;
    
    char *new_buff = remove_c(tmp, '\'');
	if(!new_buff)
		return ;
    free(tmp);
    
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        free(new_buff);
        return;
    }
    
    new_token->value = ft_strdup(new_buff);
    new_token->next = NULL;
	if (ft_strcmp(new_buff, "echo") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "-n") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "cd") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "pwd") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "export") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "unset") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "env") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, "exit") == 0)
		new_token->type = cmd;
    else if (ft_strcmp(new_buff, ">") == 0 && state == UNQUOTED)
		new_token->type = red;
    else if (ft_strcmp(new_buff, "<") == 0 && state == UNQUOTED)
		new_token->type = red;
    else if (ft_strcmp(new_buff, ">>") == 0 && state == UNQUOTED)
		new_token->type = red;
    else if (ft_strcmp(new_buff, "<<") == 0 && state == UNQUOTED)
		new_token->type = red;
    else if (ft_strcmp(new_buff, "|") == 0 && state == UNQUOTED)
		new_token->type = pip;
    else
		new_token->type = text;

	free(new_buff);

    if (*tokens == NULL) {
        *tokens = new_token;
    }
	else
	{
        t_token *current = *tokens;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}


void detect_file(t_token *tokens)
{
    t_token *current = tokens;
    t_token *second = NULL;

    while (current != NULL)
    {
        if (current->type == text && second != NULL && second->type == red)
        {
            if (ft_strcmp(second->value, ">") == 0 || 
                ft_strcmp(second->value, ">>") == 0 || 
                ft_strcmp(second->value, "<") == 0)
            {
                current->type = file;
            }
        }
        second = current;
        current = current->next;
    }
}

t_token	*sep(char *line)
{
    t_token *tokens = NULL;
    char buffer[9999];
    int i = 0;
	int j = -1;
    t_quote_state state = UNQUOTED;

	int i = -1; 
    while (line[++j])
	{
        char c = line[j];
        if (state == UNQUOTED && (c == '\'' || c == '"'))
		{
			if (c == '\'')
				state = SINGLE_QUOTED;
			else
				state = DOUBLE_QUOTED;
            buffer[i++] = c;
        }
		else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && line[j + 1] != '\'' && line[j + 1] != '"') || 
				(state == DOUBLE_QUOTED && c == '"' && line[j + 1] && line[j + 1] != '\'' && line[j + 1] != '"'))
		{
            buffer[i++] = c;
            buffer[i] = '\0';
            add_token(&tokens, buffer, state); //??? ->>>  type
            state = UNQUOTED;
            i = 0;
        }
		else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"')) || 
				(state == DOUBLE_QUOTED && c == '"' && line[j + 1] && (line[j + 1] == '\'' && line[j + 1] == '"')))
		{
			buffer[i++] = c;
			if (line[j] == '\'')
				state = SINGLE_QUOTED;
			else
				state = DOUBLE_QUOTED;
			buffer[i++] = c;
		}
		else if (state == UNQUOTED && ft_strchr("|<>", c)) // add ft_strchr from lib
		{

            if (i > 0 && buffer[0] != '<' && buffer[0] != '>') {
                buffer[i] = '\0';
                get_type_add_token(&tokens, buffer, state);
                i = 0;
            }
			else if (i > 0 && buffer[0] == '<' && c == '<')
			{
				buffer[i++] = c;
			}
			else if (i > 0 && buffer[0] == '>' && c == '>')
			{
				buffer[i++] = c;
			}
			else if (i > 0 && buffer[0] == '<' && c != '<')
			{
				buffer[i] = '\0';
				get_type_add_token(&tokens, buffer, state);
				i = 0;
			}
			else if (i > 0 && buffer[0] == '>' && c != '>')
			{
				buffer[i] = '\0';
				get_type_add_token(&tokens, buffer, state);
				i = 0;
			}
			else if (i > 0 && buffer[0] == '|' && c == '|')
			{
				buffer[i++] = c;
			}
			else if (i > 0 && buffer[0] == '|' && c != '|')
			{
				buffer[i++] = c;
				buffer[i] = '\0';
				get_type_add_token(&tokens, buffer, state);
				i = 0;
			}
			else if (i == 0)
			{
				buffer[i++] = c;
			}
        }
		else if (state == UNQUOTED && ft_isspace(c))
		{
            if (i > 0)
			{
                buffer[i] = '\0';
                get_type_add_token(&tokens, buffer, state);
                i = 0;
            }
        }
		else
            buffer[i++] = c;
    }
    if (i > 0 && state != UNQUOTED) {
        
		//exit erroor!!!!!!!!!!!!free
    }
	detect_file(tokens);
    return tokens;
}












































