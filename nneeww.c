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
int pr_check(char *buffer, t_quote_state *state)
{
	if (state == UNQUOTED && (ft_strcmp(buffer, ">") == 0 || ft_strcmp(buffer, ">>") == 0))
		return(1);
	if (state == UNQUOTED && (ft_strcmp(buffer, "<") == 0 || ft_strcmp(buffer, "<<") == 0))
		return(1);
	if (state == UNQUOTED && (ft_strcmp(buffer, "|") == 0))
		return(1);
	return (0);// write syntax error !!!!!!
}


void get_type_add_token(t_token **tokens, char *buffer, t_quote_state *state)
{

	if (buffer[0] == '>' || buffer[0] == '<' || buffer[0] == '|')
	{
		if (!pr_check(buffer, state))
			return ;
	}
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
	{
		new_token->type = cmd;
		new_token->value = "echo";
	}
    else if (ft_strcmp(new_buff, "-n") == 0)
	{
		new_token->type = cmd;
		new_token->value = "-n";		
	}
    else if (ft_strcmp(new_buff, "cd") == 0)
	{
		new_token->type = cmd;
		new_token->value = "cd";		
	}
    else if (ft_strcmp(new_buff, "pwd") == 0)
	{
		new_token->type = cmd;
		new_token->value = "pwd";		
	}
    else if (ft_strcmp(new_buff, "export") == 0)
	{
		new_token->type = cmd;
		new_token->value = "export";		
	}
    else if (ft_strcmp(new_buff, "unset") == 0)
	{
		new_token->type = cmd;
		new_token->value = "unset";		
	}
    else if (ft_strcmp(new_buff, "env") == 0)
	{
		new_token->type = cmd;
		new_token->value = "env";		
	}
    else if (ft_strcmp(new_buff, "exit") == 0)
	{
		new_token->type = cmd;
		new_token->value = "exit";		
	}
    else if (ft_strcmp(new_buff, ">") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = ">";
	}
    else if (ft_strcmp(new_buff, "<") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = "<";
	}
    else if (ft_strcmp(new_buff, ">>") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = ">>";		
	}
    else if (ft_strcmp(new_buff, "<<") == 0 && state == UNQUOTED)
	{
		new_token->type = red;
		new_token->value = "<<";		
	}
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
	detect_file(tokens);
	expand_variables(tokens, state);
}


void detect_file(t_token *tokens)
{
    t_token *current = tokens;
    t_token *previous = NULL;

    while (current != NULL)
    {
        if (current->type == text && previous != NULL && previous->type == red)
        {
            if (ft_strcmp(previous->value, ">") == 0 || 
                ft_strcmp(previous->value, ">>") == 0 || 
                ft_strcmp(previous->value, "<") == 0)
            {
                current->type = file;
            }
        }
        previous = current;
        current = current->next;
    }
}
int ft_stop(char c)
{
	return (ft_isspace(c) || c == '>' || c == '<');
}

int syntax_valid(t_token *tokens)
{
	t_token *current = tokens;
	t_token *previous;

	if (!current)
        return 1;
	previous = current;
	current = current->next;
	if (previous->type == pip || previous->type == red)
		return (0);
	while (current)
	{
		if ((previous->type == pip || (previous->type == red && ft_strcmp(previous->value, "<<") == 0)) && current->type == file )
			return (0);
		if (current->type == pip && previous->type == pip)
			return 0;
		if (current->type == red && ft_strcmp(previous->value, "<<") == 0 && (!current->next || current->next->type != file))
			return 0;
		previous = current;
    	current = current->next;
	}
	if (previous->type == pip || previous->type == red)
		return (0);
	return (1);
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
			if (c == '\'')				state = SINGLE_QUOTED;
			else
				state = DOUBLE_QUOTED;
            buffer[i++] = c;
        }
		else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && line[j + 1] != '\'' && line[j + 1] != '"' && ft_stop(line[j+1])) || 
				(state == DOUBLE_QUOTED && c == '"' && line[j + 1] && line[j + 1] != '\'' && line[j + 1] != '"' && ft_stop(line[j+1])))
		{
            buffer[i++] = c;
            buffer[i] = '\0';
            add_token(&tokens, buffer, state);
            state = UNQUOTED;
            i = 0;
        }
		else if ((state == SINGLE_QUOTED && c == '\'' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"' || !ft_stop(line[j+1]))) || 
				(state == DOUBLE_QUOTED && c == '"' && line[j + 1] && (line[j + 1] == '\'' || line[j + 1] == '"' || !ft_stop(line[j+1]))))
		{
			buffer[i++] = c;
			state = UNQUOTED;
		}
		else if (state == UNQUOTED && (ft_strchr("|<>", c) || buffer[0] == '|' || buffer[0] == '>' || buffer[0] == '<'))
		{

            if (i > 0 && buffer[0] != '<' && buffer[0] != '>')
			{
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
    return tokens;
}
