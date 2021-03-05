//
// Created by Wynn Elease on 2/28/21.
//

#include "WebServ.hpp"
#include "includes/includes.hpp"
#include "parse/ParseConfig.hpp"
#include "signal.h"
# define RED  "\e[31m"
#include "Client.h"
#include <vector>
#define notFound (size_t)-1
void fd_init(std::vector<WebServer> &servers, fd_set &fd_write, fd_set &fd_read, int &max_fd)
{
	FD_ZERO(&fd_write); // зануление fd
	FD_ZERO(&fd_read); // зануление fd
	for(std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET(it->get_server_fd(), &fd_read);
		max_fd = it->get_server_fd();
	}
}

void start_servers(std::vector<WebServer> servers) {
	std::vector<Client *> clients;
	int  			max_fd;
	fd_set			fd_write;
	fd_set			fd_read;
	char			*buf;
	int				fd;
	size_t			len;
	Bytes			tmp;
	ssize_t			ret;
	struct timeval 	t;
	std::list<Client*>::iterator tmpiter;

	if (!(buf = (char *) malloc(sizeof(char) * 256001)))
		exit(1);

	int j = 1;
	size_t p = 0;
	std::cout << GREEN << "Webserver is started" << DEFAULT<< std::endl;
	signal(SIGPIPE, SIG_IGN);
	size_t count1 = 0;
	while (j) {
		if (j++ == 0)
			break;
		fd_init(servers, fd_write, fd_read, max_fd);
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
			if ((*it)->getPhase() < responseGenerate) {
				//std::cout << GREEN << p << " to read set " << (*it)->getClientFd() << " phase: " << (*it)->getPhase() << DEFAULT << std::endl;
				FD_SET((*it)->getClientFd(), &fd_read);
			}
			else {
				//std::cout << GREEN << p << " " <<  (*it)->getClientFd() << " to write set " << DEFAULT << std::endl;
				FD_SET((*it)->getClientFd(), &fd_write);
			}
			if ((*it)->getClientFd() > max_fd)
				max_fd = (*it)->getClientFd();
			p++;
		}

		t.tv_sec = 240;
		t.tv_usec = 0;

		select(max_fd + 1, &fd_read, &fd_write, 0, &t);

		int e = 0;

		for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
			if (FD_ISSET(it->get_server_fd(), &fd_read)) {
				fd = accept(it->get_server_fd(), 0, 0);
				if (fd > 0)
					clients.push_back(new Client(*it, fd));
			}
		}

		for (std::vector<Client*>::iterator i = clients.begin(); i != clients.end();) {
			if (FD_ISSET((*i)->getClientFd(), &fd_read)) {
				bzero(buf, 256001);
				ret = recv((*i)->getClientFd(), buf, 256000, MSG_TRUNC);
				if (ret <= 0) {
					std::cout << "recv fail" << std::endl;
//					exit(0);
					(*i)->setPhase(closing);
					std::cout << "cont" << std::endl;
					i++;
					continue;
				}
				if (errno == EPIPE) {
					std::cout << "sigpipe" << std::endl;
					exit(0);
					i = clients.erase(i);
					errno = 0;
					continue;
				}
				if ((*i)->getPhase() == parsHeaders) {
					(*i)->getReceivedData()->addData(buf, ret);
					if ((*i)->getPhase() == parsHeaders && (len = (*i)->getReceivedData()->findMemoryFragment((char *)doubleCRLF, 4)) != notFound) {
						tmp = (*i)->getReceivedData()->cutData(len + 4);
						(*i)->getReceivedData()->addData((char *)"", 1);
						//std::cout << BLUE << (*i)->getReceivedData()->toPointer() << DEFAULT << std::endl;
						(*i)->setRequest(new Request((*i)->getReceivedData()->toPointer()));
						count1++;
						std::cout << "count1: " << count1 << std::endl;
						(*i)->getRequest()->setReqBody(tmp.toPointer(), tmp.getDataSize());
						(*i)->setPhase(parsBody);
						if ((*i)->getRequest()->isHeadersParsed())
							(*i)->setPhase(responseGenerate);
						//if (strcmp((*i)->getRequest()->getMetod(), "GET") == 0 && strcmp((*i)->getRequest()->getURI(), "/") == 0) {
						//	(*i)->getToSendData()->addData("HTTP/1.1 200 OK\r\n\r\n", 23);
						//	(*i)->setPhase(sendingResponse);
						//	FD_SET((*i)->getClientFd(), &fd_write);
						//	goto tmp;
						//}
						++i;
						continue;
//						goto tmp;
					}
				}
				else if ((*i)->getPhase() == parsBody) {
					(*i)->getRequest()->setReqBody(buf, ret);
					if (strcmp((*i)->getRequest()->getTransferEncoding(), "chunked") != 0 && (*i)->getRequest()->getReqBody().getDataSize() >= atoi((*i)->getRequest()->getContentLength())) {
						(*i)->getRequest()->getReqBody().cutData(atoi((*i)->getRequest()->getContentLength()));
						(*i)->setPhase(responseGenerate);
					}
					if (strcmp((*i)->getRequest()->getTransferEncoding(), "chunked") == 0 &&
							(*i)->getRequest()->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != notFound) {
						len = (*i)->getRequest()->getReqBody().findMemoryFragment("0\r\n\r\n", 5);
						(*i)->getRequest()->getReqBody().cutData(len);
						(*i)->getRequest()->ChunkedBodyProcessing();
						(*i)->setPhase(responseGenerate);
					}
				}
			}
			if ((*i)->getPhase() == responseGenerate) {
				(*i)->setResponse(new Response());
				(*i)->getToSendData()->addData((*i)->getResponse()->give_me_response(*((*i)->getRequest()), (*i)->getWebServer()), (*i)->getResponse()->getLenOfResponse());
				(*i)->setPhase(sendingResponse);
				//++i;
				//continue;
			}
			//tmp:
			if ((*i)->getPhase() == sendingResponse && FD_ISSET((*i)->getClientFd(), &fd_write)) {
				ret = send((*i)->getClientFd(), (*i)->getToSendData()->toPointer() + (*i)->getSendBytes(), (*i)->getToSendData()->getDataSize() - (*i)->getSendBytes(), 0);
				//std::cout << RED << "sending " << (*i)->getClientFd() << " " << ret << " bytes " << DEFAULT << std::endl;
				if (ret < 0) {
					exit(0);
					(*i)->setPhase(closing);
				}
				if (errno == EPIPE) {
					exit(0);
					(*i)->setPhase(closing);
					errno = 0;
				}
				(*i)->setSendBytes((*i)->getSendBytes() + ret);
				if ((*i)->getSendBytes() == (*i)->getToSendData()->getDataSize())
					(*i)->setPhase(closing);
			}
			++i;
		}

		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ) {
			if ((*it)->getPhase() == closing) {
				//std::cout << BLUE << "closing " << (*it)->getClientFd() << DEFAULT << std::endl;
				FD_CLR((*it)->getClientFd(), &fd_read);
				FD_CLR((*it)->getClientFd(), &fd_write);
				close((*it)->getClientFd());
				it = clients.erase(it);
			}
			else
				++it;
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