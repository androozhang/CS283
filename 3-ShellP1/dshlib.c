#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

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
    char *arg_ptr;
    char *token = strtok_r(cmd_line, "|", &command_ptr);
    int cmd_count = 0;

    while (token != NULL) {
        while (*token == ' ') {
            token++;
        }

        char *end_of_token = token + strlen(token) - 1;
        while (end_of_token > token && *end_of_token == ' ') {
            *end_of_token = '\0';
            end_of_token--;
        }

        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        clist->num = cmd_count;
        command_t *cmd = &clist->commands[cmd_count];

        char *arg_token = strtok_r(token, " ", &arg_ptr);
        if (arg_token == NULL || strlen(arg_token) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strncpy(cmd->exe, arg_token, EXE_MAX);
        cmd->exe[EXE_MAX - 1] = '\0';
        cmd->args[0] = '\0';
        
        // Use strtok_r again to skip the first token (executable)
        
        arg_token = strtok_r(NULL, " ", &arg_ptr);
        size_t arg_len = 0;
        while (arg_token != NULL) {
            size_t token_len = strlen(arg_token);
            if (arg_len + token_len + 1 >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            
            if (arg_len > 0) {
                cmd->args[arg_len] = ' ';
                arg_len++;
            }
            memcpy(cmd->args + arg_len, arg_token, token_len);
            arg_len += token_len;
            cmd->args[arg_len] = '\0';

            arg_token = strtok_r(NULL, " ", &arg_ptr);
        }

        cmd_count++;
        token = strtok_r(NULL, "|", &command_ptr);
    }

    clist->num = cmd_count;
    return OK;
}