#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *command_ptr;
    char *token = strtok_r(cmd_line, "|", &command_ptr);
    int cmd_count = 0;
    char *start = cmd_line;
    char *end = cmd_line + strlen(cmd_line) - 1;

    while (*start && *start == ' ')
        start++;

    while (end > start && *end == ' ')
    {
        *end = '\0';
        end--;
    }

    if (*start == '\0' || start[strlen(start) - 1] == '|')
    {
        return WARN_NO_CMDS;
    }

    clist->num = cmd_count;

    char cmd_copy[SH_CMD_MAX];

    while (token != NULL)
    {
        while (*token == ' ')
        {
            token++;
        }

        char *end_of_token = token + strlen(token) - 1;
        while (end_of_token > token && *end_of_token == ' ')
        {
            *end_of_token = '\0';
            end_of_token--;
        }
        if (*start == '\0' || (strlen(start) == 1 && *start == '|'))
        {
            return WARN_NO_CMDS;
        }

        if (cmd_count >= CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        cmd_buff_t *cmd = &clist->commands[cmd_count];
        cmd->argc = 0;

        cmd->_cmd_buffer = (char *)malloc(SH_CMD_MAX * sizeof(char));
        if (cmd->_cmd_buffer == NULL)
        {
            return ERR_MEMORY;
        }

        strncpy(cmd->_cmd_buffer, token, SH_CMD_MAX - 1);
        cmd->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

        strcpy(cmd_copy, cmd->_cmd_buffer);

        char *saveptr;
        char *arg_token = strtok_r(cmd->_cmd_buffer, " ", &saveptr);

        while (arg_token != NULL)
        {
            size_t len = strlen(arg_token);

            if (len >= 2 &&
                ((arg_token[0] == '"' && arg_token[len - 1] == '"') ||
                 (arg_token[0] == '\'' && arg_token[len - 1] == '\'')))
            {
                arg_token[len - 1] = '\0';
                arg_token++;
            }

            else if (arg_token[0] == '"' || arg_token[0] == '\'')
            {
                arg_token++;
            }
            else if (arg_token[len - 1] == '"' || arg_token[len - 1] == '\'')
            {
                arg_token[len - 1] = '\0';
            }

            if (cmd->argc >= CMD_ARGV_MAX - 1)
            {
                free(cmd->_cmd_buffer);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            cmd->argv[cmd->argc++] = arg_token;

            arg_token = strtok_r(NULL, " ", &saveptr);
        }

        cmd->argv[cmd->argc] = NULL;
        cmd_count++;
        token = strtok_r(NULL, "|", &command_ptr);
    }

    clist->num = cmd_count;
    return OK;
}

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
    int num_children = 0;
    pid_t child_pids[CMD_MAX];

    command_list_t clist;

    // TODO IMPLEMENT MAIN LOOP
    while (1)
    {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
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

        int build_cmd_result = build_cmd_list(cmd_buff, &clist);

        if (build_cmd_result == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            exit(OK);
        } else if (build_cmd_result == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
        }

        if (strcmp(input_ptr, EXIT_CMD) == 0)
        {
            free(cmd_buff);
            return OK;
        }

        if (clist.num == 1)
        {
            cmd_buff_t *cmd = &clist.commands[0];

            if (strcmp(cmd->argv[0], "cd") == 0 && cmd->argc > 1)
            {
                if (chdir(cmd->argv[1]) != 0)
                {
                    perror("cd");
                }

                continue;
            }

            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(cmd->argv[0], cmd->argv);
                perror(CMD_ERR_EXECUTE);
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                int status;
                waitpid(pid, &status, 0);
            }
            else
            {
                perror("fork");
            }

            free(cmd->_cmd_buffer);
            continue;
        }

        int pipes[CMD_MAX - 1][2];
        num_children = 0;

        for (int i = 0; i < clist.num - 1; i++)
        {
            if (pipe(pipes[i]) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < clist.num; i++)
        {
            child_pids[num_children] = fork();

            if (child_pids[num_children] == 0)
            {
                if (i > 0)
                {
                    if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
                    {
                        perror("dup2 input");
                        exit(EXIT_FAILURE);
                    }
                }

                if (i < clist.num - 1)
                {
                    if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
                    {
                        perror("dup2 output");
                        exit(EXIT_FAILURE);
                    }
                }

                for (int j = 0; j < clist.num - 1; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                execvp(clist.commands[i].argv[0], clist.commands[i].argv);
                perror(CMD_ERR_EXECUTE);
                exit(EXIT_FAILURE);
            }
            else if (child_pids[num_children] < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            num_children++;
        }

        for (int i = 0; i < clist.num - 1; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        for (int i = 0; i < num_children; i++)
        {
            waitpid(child_pids[i], NULL, 0);
        }

        for (int i = 0; i < clist.num; i++)
        {
            free(clist.commands[i]._cmd_buffer);
        }
    }

    free(cmd_buff);
    return OK;
}
