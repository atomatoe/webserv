//
// Created by Wynn Elease on 2/10/21.
//
#include "cgi.h"

int getEnv(char **env, Request request){
	env[0] = ft_strdup("SERVER_NAME=127.0.0.1"); //todo in config file
	env[1] = ft_strdup("SERVER_SOFTWARE=webserv/0.0");
	env[2] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
	env[3] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[4] = ft_strdup("SERVER_PORT=80"); //todo in config file
	env[5] = ft_strjoin("REQUEST_METOD=", request.getMetod());
	env[6] = ft_strjoin("PATH_INFO=", "/testing_cgi"); //todo path to file
	env[7] = ft_strjoin("PATH_TRANSLATED=", "~/Desktop/webserv_merged/testing_cgi/"); //todo absol path
	env[8] = ft_strjoin("SCRIPT_NAME=/cgi-bin/", "hello.cpp");
	env[9] = ft_strjoin("QUERY_STRING=", "/"); //todo from config
	env[10] = ft_strjoin("REMOTE_HOST=", request.getURI());
	env[11] = ft_strdup("REMOTE_ADDR=");
	env[12] = ft_strdup("AUTH_TYPE=basic");
	env[13] = ft_strdup("REMOTE_USER=");
	env[14] = ft_strdup("REMOTE_IDENT");
	env[15] = ft_strjoin("CONTENT_TYPE=", request.getContentType());
	env[16] = ft_strjoin("CONTENT_LENGTH=", request.getContentLength());
	env[17] = NULL;
	return 0;
}

int toCGI(Request request, char** buffWriteFd){
	char *env[18];
	char *argv[3];
	int status;
	int trumpet_fd[2];
	int fd_final = open("final", O_RDWR);
	char buf;
	char buf_big[1000];
	int i = 0;

	pipe(trumpet_fd); //todo error
	if (getEnv(env, request) == -1)
		return -1;
	argv[0] = ft_strdup("testing_cgi/cgi-bin/hello.cpp");
	argv[1] = ft_strdup("~/Desktop/webserv_merged/testing_cgi/cgi-bin/");
	argv[2] = NULL;

	pid_t pid;
	if ((pid = fork()) == 0){
		close(trumpet_fd[1]);
		dup2(trumpet_fd[0], 0);
		close(trumpet_fd[0]);
		dup2(fd_final, 1);
		close(fd_final);
		exit(execve(argv[0], argv, env));
	}
	else if (pid == -1) ;//todo error 500
	else
	{
		write(trumpet_fd[1], request.getReqString(), strlen(request.getReqString()));
		close(trumpet_fd[0]);
		close(trumpet_fd[1]);
		wait(&status);
		lseek(fd_final, 0 , 0);
		//std::cout << "HERE\n";
		while (read(fd_final, &buf, 1))
		{ //todo buf with big size
			buf_big[i] = buf;
			i++;
		}
		buf_big[i] = '\0';
		char *tmp = *buffWriteFd;
		*buffWriteFd = ft_strjoin(*buffWriteFd, buf_big); //todo leak
		//std::cout << *buffWriteFd << std::endl;
		free(tmp);
	}
	return 0;
}

