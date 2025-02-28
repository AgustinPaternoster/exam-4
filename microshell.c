#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void err_msg(char *msg)
{
    write(2,msg, strlen(msg));
}

void err_exit(char *msg, int status)
{
    write(2,msg, strlen(msg));
    exit(status);
}

int my_cd(char **argv, int i)
{
    if (i != 2)
    {
        err_msg("error: cd: bad arguments\n");
        return(1);
    } 
    if (chdir(argv[1]) == -1)
    {
        err_msg("error: cd: cannot change directory to ");
        err_msg("argv[1]");
        err_msg("\n");
        return(1);
    }
    return (0);
}

int has_pipe(char **argv, int i)
{
    if (argv[i] && strncmp(argv[i], "|" , 2))
        return (1);
    return (0);
}

void set_pipe(int *fd, int end, int haspipe)
{
    if (haspipe)
    {
        if(dup2(fd[end], end) == -1)
            err_exit("error: fatal\n", 1);
        if(close(fd[0]) == -1)
            err_exit("error: fatal\n", 1);
        if(close(fd[1]) == -1)
            err_exit("error: fatal\n", 1);
    }
}

int exec(char **argv, char **env, int i)
{
    int haspipe;
    int fd[2];
    int status;
    int pid;

    haspipe = has_pipe(argv, i);
    if (!haspipe && !strncmp(argv[0], "cd", 3))
        return(my_cd(argv, i));
    if (haspipe && pipe(fd) == -1)
        err_exit("error: fatal\n", 1);
    if ((pid=fork()) == -1)
        err_exit("error: fatal\n", 1);
    if (!pid)
    {
        argv[i] = 0;
        set_pipe(fd, 1, haspipe);
        if (!strncmp(argv[0], "cd", 3))
            exit(my_cd(argv,i));
        execve(argv[0], argv , env);
        err_msg("error: cannot execute");
        err_msg(argv[0]);
        err_msg("\n");
        exit(1);
    }
    waitpid(pid, &status,0);
    set_pipe(fd,0,haspipe);
    return (WIFEXITED(status) && WEXITSTATUS(status));
}


int main (int argc, char** argv, char ** env)
{
    int i;
    int status;
    (void)argc;

    i = 0;
    while (argv[i])
    {
        argv += i + 1;
        while (argv[i] && strncmp(argv[i], "|", 2) && strncmp(argv[i], ";", 2))
            i++;
        if (i)
            status = exec(argv,env, i);
    }
    return (status);
}