/*--------------------------------------

    Copyright Vlad Ionut Ursachi 2016
    Telnet based C++ Chat Server

----------------------------------------*/

#include "platform.h"

int check_number(char* str, int& out)
{
    out = DEFAULT_PORT_NUMBER;

    //check if the parameter contains digits
    int i = 0;
    int found = FALSE;
    while (str[i] != '\0')
    {
        if (isdigit(str[i]) != 0)
        {
            found = TRUE;
            break;
        }
        //go to next char
        i++;
    }

    if (found)
    {
        out = atoi(str);
        return CS_OK;
    }
    else
        return CS_INVALID_ARGS;
}

int parse_arguments(int argc, char* argv[], FILE* logfile, int& port_number)
{
    int i;

    printf("Usage: ChatServer [-port port_number] [-log log_file_name]\n");

    //check number of parameters
    if (argc != 1 && argc != 3 && argc != 5)
    {
        printf("Invalid number of parameters\n");
        return CS_INVALID_ARGS;
    }

    for (i = 1; i < argc - 1; i++)
    {
        //check arguments
        if (strcmp(argv[i], "-port") == 0)
        {
            //parse port number and exit in case of error
            if (check_number(argv[i + 1], port_number) == CS_INVALID_ARGS)
            {
                printf("Invalid port number given\n");
                return CS_INVALID_ARGS;
            }
        }
        if (strcmp(argv[i], "-log") == 0)
        {
            logfile = fopen(argv[i + 1], "at");
            if (logfile == NULL)
            {
                printf("Invalid log filename given\n");
                return CS_INVALID_ARGS;
            }
        }
    }

    return CS_OK;
}

int main(int argc, char* argv[])
{    
    int port_number = DEFAULT_PORT_NUMBER;
    FILE* logfile = NULL;

    //parse arguments
    if (parse_arguments(argc, argv, logfile, port_number) != CS_OK)
        return CS_INVALID_ARGS;

    //Server chatserver;

    //if (chatserver.try_on(port_number) != 0)
    //{
    //    printf("Starting on %d port number\n", port_number);
    //    chatserver.start_on(port_number);
    //}
    //else
    //{
    //    printf("Port number %d in use!\n");
    //}

    return CS_OK;
}