#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define WRITE_END 1
#define READ_END 0

#define STDIN 0
#define STDOUT 1
#define STDERROR 2

//https://github.com/Bima42/42-exam-rank/blob/main/exam_04/microshell.c
// 1 parsear los argunemtos
// 2 ejecutar argumentos
// 

void fix_error(char ** argv , int argc)
{
	int i = 0;
	while (i < argc)
	{
		if (!strncmp(argv[i], "P", 2))
				strcpy(argv[i], "|");
		i++;
	}
}
void err_exit(char *msg, int status)
{
	write(STDERR_FILENO, msg, strlen(msg));
	exit(status);
}

void err(char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
}

int my_cd( char **argv, int arg)
{
	
	if (arg !=2)
		return(err("error: cd: bad arguments\n"), 1);
	if (chdir(argv[1]) !=0)
	{
		err("error: cd: cannot change directory to ");
		err(argv[1]);
		err("\n");
		return (1);
	}
	return (0);
}

void set_pipe(int haspipe, int *fd, int pid)
{
	if (haspipe)
	{
		if (pid = 0)
		{
			if(close(fd[READ_END] == -1))
				err_exit("error: fatal\n",1);
			if(dup2(fd[WRITE_END],STDOUT_FILENO) == -1 )
				err_exit("error: fatal\n",1);
			if(close(fd[WRITE_END] == -1))
				err_exit("error: fatal\n",1);
		}
		else
		{
			if(dup2(fd[READ_END],STDIN_FILENO) == -1 )
				err_exit("error: fatal\n",1);
			if(close(fd[READ_END] == -1))
				err_exit("error: fatal\n",1);
			if(close(fd[WRITE_END] == -1))
				err_exit("error: fatal\n",1);
		}
	}
}
int has_pipe(char **argv)
{
	int i = 0;
	int ret = 0;

	while(argv[i])
	{
		if (!strncmp(argv[i], "|", 2))
			return (1);
		if (!strncmp(argv[i], ";", 2))
			return (0);
		i++;
	}
	return (ret);
}


int exec(char **argv, char **env, int i)
{
	int haspipe = has_pipe(argv);
	int status;
	int pid;
	int fd[2];
	
	if (!haspipe && !strncmp(argv[0],"cd", 3))
		return (my_cd(argv, i));
	if (has_pipe && pipe(fd) == -1)
		err_exit("error: fatal\n",1);
	if (pid = fork() == -1)
		err_exit("error: fatal\n",1);

	
	return (0);
}

int main (int argc, char **argv, char **env)
{
	int i;
	int status;

	(void)argc;
	(void)env;
	fix_error(argv, argc);
	i = 0;
	status = 0;
	(void)i;

	while (argv[i])
	{
		argv += i + 1;
		i = 0;
		while (argv[i] && strncmp(argv[i], "|", 2) && strncmp(argv[i], ";", 2))
			i++;	
		if (i)
			status = exec(argv, env, i);
	}
	printf("%d\n", status);
	return (0);
}