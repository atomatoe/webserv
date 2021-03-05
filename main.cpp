//
// Created by Wynn Elease on 2/22/21.
//

#include "WebServ.hpp"
#include "./Request_Response/Request.hpp"
#include "./Request_Response/Response.hpp"
#include "includes/includes.hpp"
#include "parse/ParseConfig.hpp"
#include "signal.h"
# define RED  "\e[31m"
void fd_init(std::vector<WebServer> &servers, fd_set &fd_write, fd_set &fd_read, int &max_fd)
{
	FD_ZERO(&fd_write); // зануление fd
	FD_ZERO(&fd_read); // зануление fd
	for(std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET(it->get_server_fd(), &fd_read);
		max_fd = it->get_server_fd();
	}
}

void closeClientFd(std::map<int, t_client>::iterator * i, fd_set fd_write, fd_set fd_read, WebServer *server){
	//FD_CLR((*i)->first, &fd_read);
	//FD_CLR((*i)->first, &fd_write);
	close((*i)->first);
	//delete (*i)->second.request;
	//delete (*i)->second.toSendData;
	//delete (*i)->second.response;
	//delete (*i)->second.receivedData;
	*i = server->getClients().erase(*i);
}



void start_servers(std::vector<WebServer> servers) {
    int			max_fd;
    fd_set		fd_write;
    fd_set		fd_read;
    char		*buf;
    int			fd;
    Bytes		tmp;
    size_t		len;
    ssize_t			ret;
    size_t count1 = 0;
    struct timeval t;
    size_t p = 0;
    if (!(buf = (char *) malloc(sizeof(char) * 512001)))
        exit(1);

    int j = 1;
    std::cout << GREEN << "Webserver is started" << DEFAULT<< std::endl;
    int count = 0;
	signal(SIGPIPE, SIG_IGN);
    while (j) {
		if (j++ == 0)
			break;
		fd_init(servers, fd_write, fd_read, max_fd);

		for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
			for (std::map<int, t_client>::iterator i = it->getClients().begin(); i != it->getClients().end(); ++i) {
				if (i->second.phase < 2) {
					std::cout << GREEN << p << " to read set " << i->first << " phase: " << i->second.phase << DEFAULT << std::endl;
					FD_SET(i->first, &fd_read);
				} else {
					std::cout << GREEN << p << i->first << " to write set " << i->first << DEFAULT << std::endl;
					FD_SET(i->first, &fd_write);
				}
				if (i->first > max_fd)
					max_fd = i->first;
				p++;
			}
		}

		t.tv_sec = 240;
		t.tv_usec = 0;

		if (select(max_fd + 1, &fd_read, &fd_write, 0, &t) < 0) {
			std::cout << "end of cycle" << std::endl;
			break;
		};

		for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
			for (std::map<int, t_client>::iterator i = it->getClients().begin(); i != it->getClients().end(); ++i)
			{
				if (i->second.phase == 5)
				{
					std::cout << BLUE << "closing " << i->first << DEFAULT << std::endl;
					FD_CLR(i->first, &fd_read);
					FD_CLR(i->first, &fd_write);
					close(i->first);
					i = it->getClients().erase(i);
					if (i == it->getClients().end())
						break;
				}
			}
		}

		for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
			if (FD_ISSET(it->get_server_fd(), &fd_read)) {
				fd = accept(it->get_server_fd(), 0, 0);
				if (fd > 0)
					it->addClient(fd);
			}
		}

		for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
			std::map<int, t_client>::iterator i = it->getClients().begin();
			while (i != it->getClients().end()) {
				if (FD_ISSET(i->first, &fd_read)) {
					bzero(buf, 512001);
					ret = recv(i->first, buf, 512000, MSG_TRUNC);
					if (ret <= 0) {
						std::cout << "recv fail" << std::endl;
						//exit(0);
						closeClientFd(&i, fd_write, fd_read, &*it);
						std::cout << "cont" << std::endl;
						continue;
					}
					if (errno == EPIPE) {
						std::cout << "sigpipe" << std::endl;
						exit(0);
						closeClientFd(&i, fd_write, fd_read, &*it);
						errno = 0;
						continue;
					}
					if (!i->second.phase && ret > 0) {
						i->second.receivedData->addData(buf, ret);
						if (i->second.phase < 1 && (len = i->second.receivedData->findMemoryFragment((char *) doubleCRLF, 4)) != (size_t) -1) {
							count++;
							tmp = i->second.receivedData->cutData(len + 4);
							i->second.receivedData->addData((char *) "", 1);
							//std::cout << BLUE << i->second.receivedData->toPointer() << DEFAULT << std::endl;
							i->second.request = new Request(i->second.receivedData->toPointer());
							count1++;
							std::cout << i->first << " count1: " << count1 << std::endl;
							i->second.request->setReqBody(tmp.toPointer(), tmp.getDataSize());
							i->second.phase = 1;
							if (strcmp(i->second.request->getMetod(), "GET") == 0 || strcmp(i->second.request->getMetod(), "HEAD") == 0) {
								i->second.phase = 2;
							}
							if (strcmp(i->second.request->getTransferEncoding(), "chunked") == 0) {
								if (i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != (size_t) -1) {
									len = i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5);
									i->second.request->getReqBody().cutData(len);
									i->second.request->ChunkedBodyProcessing();
									i->second.phase = 2;
								}
								i++;
								continue;
							}
							if (i->second.request->getReqBody().getDataSize() > atoi(i->second.request->getContentLength())) {
								i->second.request->getReqBody().cutData(atoi(i->second.request->getContentLength()));
								i->second.phase = 2;
							}
							//i++;
							//continue;
						}
					} else if (ret > 0 && i->second.phase == 1) {
						i->second.request->setReqBody(buf, ret);
						if (i->second.request->getReqBody().getDataSize() >= atoi(i->second.request->getContentLength()) && strcmp(i->second.request->getTransferEncoding(), "chunked") != 0) {
							i->second.phase = 2;
							i->second.request->getReqBody().cutData(atoi(i->second.request->getContentLength()));
							//i++;
							//continue;
						}
						if (strncmp(i->second.request->getTransferEncoding(), "chunked", 7) == 0 && (i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != (size_t) -1)) {
							len = i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5);
							i->second.request->getReqBody().cutData(len);
							i->second.request->ChunkedBodyProcessing();

							i->second.phase = 2;
							//i++;
							//continue;
						}
					}
				}
				if (i->second.phase == 2) {
					i->second.response = new Response();
					i->second.toSendData->addData(i->second.response->give_me_response(*(i->second.request), *it), i->second.response->getLenOfResponse());
					i->second.phase = 3;
				}
				if (i->second.phase == 3) {
					if (i->second.sendBytes < i->second.toSendData->getDataSize()) {
						if (FD_ISSET(i->first, &fd_write)) {
						//	std::cout << i->first << " in response set" << std::endl;
							ssize_t r;
							r = send(i->first, i->second.toSendData->toPointer() + i->second.sendBytes, i->second.toSendData->getDataSize() - i->second.sendBytes, 0);
							//std::cout << r << " sended bytes\n";
							std::cout << RED << "sending " << i->first << " " << r << "bytes " << DEFAULT << std::endl;
							if (r < 0) {
								std::cout << "ERROR\n";
								//exit(0);
								closeClientFd(&i, fd_write, fd_read, &*it);
								continue;
							}
							if (errno == EPIPE) {
								std::cout << "sigpipe" << std::endl;
								//exit(0);
								closeClientFd(&i, fd_write, fd_read, &*it);
								errno = 0;
								continue;
							}
							i->second.sendBytes += r;
							if (i->second.sendBytes == i->second.toSendData->getDataSize()) {
								i->second.phase = 5;
							}
						}
					}
				}
				i++;
			}
		}
	}
}

int main(int ac, char **av) {
    signal(SIGINT, exitSignal);
    signal(SIGTERM, exitSignal);
    signal(SIGQUIT, exitSignal);

	ParseConfig parseConfig((ac == 2) ? av[1] : "configs/atomatoe/default.conf");
	std::vector<WebServer> servers =  parseConfig.parse();
	size_t count_server = servers.size();

	init_servers(servers, count_server);
	start_servers(servers);
	return(0);
}