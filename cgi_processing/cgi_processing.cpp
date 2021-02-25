//
// Created by Wynn Elease on 2/21/21.
//

#include "cgi_utils.h"

int getEnv(char **env, Request request, WebServer server){
	env[0] = ft_strjoin("SERVER_NAME=", server.getIp().c_str());
	env[1] = ft_strdup("SERVER_SOFTWARE=webserv/0.0");
	env[2] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
	env[3] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[4] = ft_strjoin("SERVER_PORT=", std::to_string(server.getPort()).c_str());
	env[5] = ft_strjoin("REQUEST_METHOD=", request.getMetod());
	env[6] = ft_strjoin("PATH_INFO=", request.getURI());
	env[7] = ft_strjoin("PATH_TRANSLATED=", request.getPathToCgi().c_str());
	env[8] = ft_strjoin("SCRIPT_NAME=", strrchr(request.getPathToCgi().c_str(), '/'));
	env[9] = ft_strjoin("QUERY_STRING=", request.getQueryString().c_str());
	env[10] = ft_strjoin("REMOTE_HOST=", request.getURI());
	env[11] = ft_strdup("REMOTE_ADDR=");
	env[12] = ft_strdup("AUTH_TYPE=basic");
	env[13] = ft_strdup("REMOTE_USER=");
	env[14] = ft_strdup("REMOTE_IDENT");
	env[15] = ft_strjoin("CONTENT_TYPE=", request.getContentType());
	env[16] = ft_strjoin("CONTENT_LENGTH=", request.getQueryString() == "" ? request.getContentLength() : std::to_string(request.getQueryString().length()).c_str());
	env[17] = ft_strjoin("REQUEST_URI=http://" ,  (server.getIp() + ":" + std::to_string(server.getPort()) + request.getURI()).c_str());
	env[18] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
	env[19] = NULL;
	return 0;
}

void toCGI(Response &response, Request request, WebServer server){
	char *env[20];
	char *argv[3];
	int status;
	int trumpet_fd[2];
	int fd_final = open("final", O_CREAT | O_RDWR | O_TRUNC, 0666);
	char buf;
	char *buf_big = (char *)malloc(100000);
	int i = 0;

	pipe(trumpet_fd); //todo error
	getEnv(env, request, server);
	argv[0] = ft_strdup(request.getPathToCgi().c_str());
	argv[1] = ft_strdup(request.getPathToCgi().c_str());
	argv[2] = NULL;
	pid_t pid;
	if ((pid = fork()) == 0) {
		close(trumpet_fd[1]);
		dup2(trumpet_fd[0], 0);
		close(trumpet_fd[0]);
		dup2(fd_final, 1);
		close(fd_final);
		exit(execve(argv[0], argv, env));
	}
	else if (pid == -1) ;//todo error 500
	else {
		write(trumpet_fd[1], request.getReqString(), strlen(request.getReqString()));
		close(trumpet_fd[0]);
		close(trumpet_fd[1]);
		wait(&status);
		//std::cout << "___" <<  status << "___" << std::endl;
		lseek(fd_final, 0 , 0);
		int ret, size = 0;
		while ((ret = read(fd_final, &buf, 1)) > 0) {
			size += ret;
			buf_big[i] = buf;
			i++;
		}
		response._bodyOfResponse.clear();
		response._bodyOfResponse.addData(buf_big, size);
	}
	//return 0;
}