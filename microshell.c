#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define SIDE_OUT 0
#define SIDE_IN 1

#define STDIN 0
#define STDOUT 1
#define STDERROR 2


// 1 parsear los argunemtos
// 2 ejecutar argumentos
// 

int ft_strlen(char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void err(char *msg, int exit_code)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	exit(exit_code);
}

int main (int argc, char **argv, char **env)
{
	int i = 0;
	int status = 0;

	while (argv[i])
		printf("%s\n", argv[i++]);
	return (0);
}