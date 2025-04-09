#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(void)
{
    char *input;

    while (1)
    {
        // Display prompt and wait for user input
        input = readline("minishell> ");

        // If user pressed Ctrl+D (EOF), exit
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // If the input is not empty, add it to history
        if (*input != '\0')
            add_history(input);

        // You can process the command here
        printf("You entered: %s\n", input);

        // Free the input line (readline allocates memory)
        free(input);
    }

    return 0;
}
