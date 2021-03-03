//
// Created by Wynn Elease on 2/22/21.
//

#include "WebServ.hpp"
#include "./Request_Response/Request.hpp"
#include "./Request_Response/Response.hpp"
#include "includes/includes.hpp"
#include "parse/ParseConfig.hpp"
#include "signal.h"

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
	FD_CLR((*i)->first, &fd_read);
	FD_CLR((*i)->first, &fd_write);
	close((*i)->first);
	*i = server->getClients().erase(*i);
}

void start_servers(std::vector<WebServer> servers)
{
    int			max_fd;
    fd_set		fd_write;
    fd_set		fd_read;
    char		*buf;
    int			fd;
    Bytes		tmp;
    size_t		len;
    int			ret;
    struct timeval tv;
    if (!(buf = (char *) malloc(sizeof(char) * 20000001)))
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
                if (i->second.phase < 2)
					FD_SET(i->first, &fd_read);
                else
					FD_SET(i->first, &fd_write);
                if (i->first > max_fd)
                    max_fd = i->first;
            }
        }

        if (select(max_fd + 1, &fd_read, &fd_write, NULL, NULL) == -1)
        	std::cout << BLUE << "ERROR" << DEFAULT << std::endl;
        for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
            if (FD_ISSET(it->get_server_fd(), &fd_read)) {
                fd = accept(it->get_server_fd(), 0, 0);
                if (fd > 0)
					it->addClient(fd);
            }
        }

        for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it)
        {
            std::map<int, t_client>::iterator i = it->getClients().begin();
            while (i != it->getClients().end()) {
            	std::cout << "size:   " << it->getClients().size() << std::endl;
                if (FD_ISSET(i->first, &fd_read)) {
                	bzero(buf, 2000001);
                    ret = recv(i->first, buf, 2000000, 0);
                    if (!i->second.phase && ret > 0) {
                        i->second.receivedData->addData(buf, ret);
                        if (i->second.phase < 1 && (len = i->second.receivedData->findMemoryFragment((char *)doubleCRLF, 4)) != (size_t) -1) {
                        	count++;
                            tmp = i->second.receivedData->cutData(len + 4);
                            i->second.receivedData->addData((char *)"", 1);
                           // std::cout << BLUE << i->second.receivedData->toPointer() << DEFAULT << std::endl;
                            i->second.request = new Request(i->second.receivedData->toPointer());
                            i->second.request->setReqBody(tmp.toPointer(), tmp.getDataSize());
                            i->second.phase = 1;
                            if (strcmp(i->second.request->getMetod(), "GET") == 0 || strcmp(i->second.request->getMetod(), "HEAD") == 0) {
                            	i->second.phase = 2;
                            }
							else if (strcmp(i->second.request->getTransferEncoding(), "chunked") == 0){
							//	std::cout << GREEN << i->second.request->getReqBody().toPointer() << DEFAULT << std::endl;
							//	std::cout << "HERREEEEEE\n";
								if (i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != (size_t) -1) {
									len = i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5);
									i->second.request->getReqBody().cutData(len);
									//std::cout << "CHUNK: " << RED << i->second.request->getReqBody().toPointer() << DEFAULT << std::endl;
									i->second.request->ChunkedBodyProcessing();

									i->second.phase = 2;
								}
								i++;
								continue;
							}
                            else if (i->second.request->getReqBody().getDataSize() > ft_atoi(i->second.request->getContentLength())){
								i->second.request->getReqBody().cutData(ft_atoi(i->second.request->getContentLength()));
								i->second.phase = 2;
                            }
                            i++;
							continue;
                        }
                    }
                    else if (ret > 0 && i->second.phase == 1) {
						//std::cout << GREEN << j << " " <<  ret << " working\n" << DEFAULT << std::endl;
                        i->second.request->setReqBody(buf, ret);
						if (i->second.request->getReqBody().getDataSize() >= ft_atoi(i->second.request->getContentLength()) && strcmp(i->second.request->getTransferEncoding(), "chunked") != 0){
							i->second.phase = 2;
							i->second.request->getReqBody().cutData(ft_atoi(i->second.request->getContentLength()));
							i++;
							continue;
						}
                        if (strncmp(i->second.request->getTransferEncoding(), "chunked", 7) == 0 && (i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != (size_t) -1)) {
                        	len = i->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5);
                        	i->second.request->getReqBody().cutData(len);
                            i->second.request->ChunkedBodyProcessing();

                            i->second.phase = 2;
                           	i++;
							continue;
                        }
                    }
                    if (ret == 0) {
                    //	std::cout << "In ret == 0\n";
                        i++;
						continue;
                    }
                }
				if (i->second.phase == 2) {
					//std::cout << "response adding\n";
					i->second.response = new Response();
					i->second.toSendData->addData(i->second.response->give_me_response(*(i->second.request), *it), i->second.response->getLenOfResponse());
					i->second.toSendData->addData((char *) doubleCRLF, 4);
					i->second.phase = 3;
				}
				if (i->second.phase == 3) {
					std::cout << "sending\n";
					gettimeofday(&tv, NULL);
					if (i->second.sendBytes < i->second.toSendData->getDataSize()) {
						if (FD_ISSET(i->first, &fd_write)) {
							i->second.sendBytes += send(i->first, i->second.toSendData->toPointer() + i->second.sendBytes, i->second.toSendData->getDataSize() - i->second.sendBytes, 0);
							if (errno == EPIPE) {
								closeClientFd(&i, fd_write, fd_read, &*it);
								errno = 0;
								continue;
							}
							i++;
							continue;
						}
						else {
							closeClientFd(&i, fd_write, fd_read, &*it);
							continue;
						}
					}
					//std::cout << "REQUEST " << count << ": " << i->second.request->getReqBody().toPointer() << "++++++" << std::endl;
					char *m = ft_memjoin(i->second.response->give_me_response(*(i->second.request), *it), "", i->second.response->getLenOfResponse(), 1);

					//std::cout << "RESPONSE: " << GREEN << m << DEFAULT << std::endl;
					closeClientFd(&i, fd_write, fd_read, &*it);
				}
				else
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