//
// Created by Wynn Elease on 2/21/21.
//

#include "utilsOfCGI.hpp"

int getEnv(char **env, Request & request, WebServer & server){
	env[0] = ft_strjoin("SERVER_NAME=", server.getIp().c_str());
	env[1] = ft_strdup("SERVER_SOFTWARE=webserv/0.0");
	env[2] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
	env[3] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[4] = ft_strjoin("SERVER_PORT=", std::to_string(server.getPort()).c_str());
	env[5] = ft_strjoin("REQUEST_METHOD=", request.getMetod().c_str());
	env[6] = ft_strjoin("PATH_INFO=", request.getURI().c_str());
	env[7] = ft_strjoin("PATH_TRANSLATED=", request.getPathToCgi().c_str());
	env[8] = ft_strjoin("SCRIPT_NAME=", strrchr(request.getPathToCgi().c_str(), '/'));
	env[9] = ft_strjoin("QUERY_STRING=", request.getQueryString().c_str());
	env[10] = ft_strjoin("REMOTE_HOST=", request.getURI().c_str());
	env[11] = ft_strdup("REMOTE_ADDR=");
	env[12] = ft_strdup("AUTH_TYPE=basic");
	env[13] = ft_strdup("REMOTE_USER=");
	env[14] = ft_strdup("REMOTE_IDENT");
	env[15] = ft_strjoin("CONTENT_TYPE=", request.getContentType().c_str());
	env[16] = ft_strjoin("CONTENT_LENGTH=", request.getQueryString() == "" ? request.getContentLength().c_str() : std::to_string(request.getQueryString().length()).c_str());
	env[17] = ft_strjoin("REQUEST_URI=http://" ,  (server.getIp() + ":" + std::to_string(server.getPort()) + request.getURI()).c_str());
	env[18] = ft_strdup("HTTP_X_SECRET_HEADER_FOR_TEST=1");
	env[19] = NULL;
	return 0;
}

void freeMass(char **mass, int size) {
	for (int i = 0; i < size; ++i)
		free(mass[i]);
}

void getBodyFromCGI(Request & request, Response &response, int &fd_final, int *trumpet_fd) {
	char	*tmp;
	int		status;
	int		ret;
	char *	buf = (char *)malloc(bufSize);

	tmp = request.getReqBody().toPointer();
	write(trumpet_fd[1], tmp, request.getReqBody().getDataSize());
	free(tmp);
	close(trumpet_fd[0]);
	close(trumpet_fd[1]);
	wait(&status);
	lseek(fd_final, 0 , 0);
	while ((ret = read(fd_final, buf, bufSize)) > 0)
		response.getBodyOfResponse().addData(buf, ret);
	free(buf);
}

void toCGI(Response &response, Request & request, WebServer & server){
	char *	env[envSize];
	char *	argv[argvSize];
	int 	trumpet_fd[2];
	int 	fd_final = open("final", O_CREAT | O_RDWR | O_TRUNC, 0666);

	pipe(trumpet_fd);
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
	else if (pid == -1)
		throw std::exception();
	else {
		getBodyFromCGI(request, response, fd_final, trumpet_fd);
		freeMass(env, envSize - 1);
		freeMass(argv, argvSize - 1);
	}
}