#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;
    cmd_buff_t cmd;
    int c_result;

    // TODO IMPLEMENT MAIN LOOP
    while (1)
    {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        char *input_ptr = cmd_buff;

        while (*input_ptr == ' ')
        {
            input_ptr++;
        }

        if (*input_ptr == '\0')
        {
            printf("%s\n", CMD_WARN_NO_CMD);
            continue;
        }
        if (strcmp(input_ptr, EXIT_CMD) == 0)
        {
            free(cmd_buff);
            return OK;
        }

        cmd.argc = 0;
        char *token = input_ptr;
        bool in_quotes = false;

        while (*token)
        {
            while (*token == ' ' && !in_quotes)
            {
                token++;
            }
            if (*token == '"')
            {
                in_quotes = true;
                token++;
            }

            if (*token == '\0')
                break;

            cmd.argv[cmd.argc++] = token;
            if (cmd.argc >= CMD_ARGV_MAX)
            {
                printf("%s\n", CMD_ERR_PIPE_LIMIT);
                return ERR_TOO_MANY_COMMANDS;
            }

            while (*token && (in_quotes || *token != ' '))
            {
                if (*token == '"')
                {
                    *token = '\0';
                    in_quotes = false;
                }
                token++;
            }

            if (*token)
            {
                *token = '\0';
                token++;
            }
        }
        cmd.argv[cmd.argc] = NULL;

        if (strcmp(cmd.argv[0], "cd") == 0)
        {
            if (cmd.argc > 1)
            {
                int chdir_result = chdir(cmd.argv[1]);

                if ((chdir_result) != 0)
                {
                    perror("cd");
                }
            }
            continue;
        }

        int f_result = fork();

        if (f_result == 0)
        {
            rc = execvp(cmd.argv[0], cmd.argv);
            if (rc < 0)
            {
                printf("%s\n", CMD_ERR_EXECUTE);
                exit(42);
            }
            exit(1);
        }
        else if (f_result == -1)
        {
            printf("Fork failed\n");
            printf("[p] The child exit status is %d\n", WEXITSTATUS(c_result));
            wait(&c_result);
        }
        else if (f_result > 0)
        {
            wait(&c_result);
        }
    }
    free(cmd_buff);

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}
