
t_types determine_meta_type(char c) {
    if (c == '|') return pip;
    if (c == '<' || c == '>') return red;
    return text;
}

void parse_tokens(t_token *tokens) {
    t_token *current = tokens;
    int expect_file = 0; // After redirection, next token is a file

    while (current) {
        if (current->type == red) {
            // Next token is a file
            if (current->next) {
                current->next->type = file;
                current = current->next; // Skip file token
            } else {
                printf("Error: Missing filename after redirection\n");
            }
        } else if (current->type == text && !expect_file) {
            // First text token is the command
            current->type = cmd;
        }
        current = current->next;
    }
}



void expand_env_vars(t_token *tokens) {
    for (t_token *t = tokens; t; t = t->next) {
        if (t->type != text && t->type != cmd && t->type != file) continue;
        char *value = t->value;
        char expanded[1024] = {0};
        t_quote_state state = UNQUOTED;

        for (int i = 0, j = 0; value[i]; i++) {
            if (value[i] == '\'' && state != DOUBLE_QUOTED)
                state = (state == SINGLE_QUOTED) ? UNQUOTED : SINGLE_QUOTED;
            else if (value[i] == '"' && state != SINGLE_QUOTED)
                state = (state == DOUBLE_QUOTED) ? UNQUOTED : DOUBLE_QUOTED;
            else if (value[i] == '$' && state != SINGLE_QUOTED) {
                // Expand variable
                char var[256] = {0};
                int k = 0;
                i++;
                if (value[i] == '?') {
                    // Handle $?
                    snprintf(var, sizeof(var), "%d", g_exit_status);
                    i++;
                } else {
                    while (isalnum(value[i]) || value[i] == '_')
                        var[k++] = value[i++];
                    i--;
                    char *env_val = getenv(var);
                    if (env_val) strcpy(var, env_val);
                }
                strcat(expanded, var);
                j += strlen(var);
            } else {
                expanded[j++] = value[i];
            }
        }
        free(t->value);
        t->value = strdup(expanded);
    }
}






typedef struct s_cmd {
    char **args;       // Command + arguments
    char *input_file;  // From < or <<
    char *output_file; // From > or >>
    struct s_cmd *next;
} t_cmd;

t_cmd *build_commands(t_token *tokens) {
    t_cmd *head = NULL;
    t_cmd **current = &head;
    t_cmd *cmd = malloc(sizeof(t_cmd));
    *current = cmd;

    while (tokens) {
        if (tokens->type == pip) {
            // New command in pipeline
            cmd->next = malloc(sizeof(t_cmd));
            cmd = cmd->next;
            tokens = tokens->next;
        } else if (tokens->type == red) {
            // Handle redirection
            if (strcmp(tokens->value, "<") == 0) {
                cmd->input_file = tokens->next->value;
            } else if (strcmp(tokens->value, ">") == 0) {
                cmd->output_file = tokens->next->value;
            }
            tokens = tokens->next->next; // Skip redirection token + file
        } else {
            // Add argument to command
            add_arg(&cmd->args, tokens->value);
            tokens = tokens->next;
        }
    }
    return head;
}

void validate_syntax(t_token *tokens) {
    t_quote_state state = UNQUOTED;
    for (t_token *t = tokens; t; t = t->next) {
        // Check for unclosed quotes
        for (int i = 0; t->value[i]; i++) {
            if (t->value[i] == '\'' && state != DOUBLE_QUOTED)
                state = (state == SINGLE_QUOTED) ? UNQUOTED : SINGLE_QUOTED;
            else if (t->value[i] == '"' && state != SINGLE_QUOTED)
                state = (state == DOUBLE_QUOTED) ? UNQUOTED : DOUBLE_QUOTED;
        }
    }
    if (state != UNQUOTED) {
        printf("Error: Unclosed quote\n");
        exit(1);
    }
}