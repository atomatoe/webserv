#include "WebServer/WebServ.hpp"
#include "../Includes/Includes.hpp"
#include "ParseConfig/ParseConfig.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "signal.h"
# define RED  "\e[31m"
# define YELLOW "\e[1;33m"
# define LMAGENTA "\e[1;35m"
# define LBLUE "\e[1;34m"
# define BUFSIZE 512000
#include "Client/Client.h"
#include <vector>
#define NOTFOUND (size_t)-1
static size_t 				g_count = 1;

void fd_init(std::vector<WebServer> &servers, fd_set &writeSet, fd_set &readSet, int & maxFd) {
	FD_ZERO(&writeSet);
	FD_ZERO(&readSet);
	for(std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET(it->get_server_fd(), &readSet);
		maxFd = it->get_server_fd();
	}
}

void settingToFdSets(std::list<Client *> & clients, int &max_fd, fd_set & writeSet, fd_set & readSet) {
	for (std::list<Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
		FD_SET((*it)->getClientFd(), &readSet);
		if ((*it)->getPhase() >= responseGenerate) {
			FD_SET((*it)->getClientFd(), &writeSet);
		}
		if ((*it)->getClientFd() > max_fd)
			max_fd = (*it)->getClientFd();
	}

}

void checkNewClients(std::vector<WebServer> & servers, fd_set & fd_read, std::list<Client *> & clients) {
	int				fd;
	for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it)
		if (FD_ISSET(it->get_server_fd(), &fd_read)) {
			fd = accept(it->get_server_fd(), 0, 0);
			if (fd > 0) {
				clients.push_front(new Client(*it, fd));
				std::cout << YELLOW << "Client successfully connected! ٩(•̤̀ᵕ•̤́๑)૭✧" << DEFAULT << std::endl;
			}
		}
}

void closingConnections(std::list<Client *> & clients) {
	for (std::list<Client*>::iterator it = clients.begin(); it != clients.end(); ) {
		if ((*it)->getPhase() == closing) {
			std::cout << LBLUE << "Connection with client with fd " << (*it)->getClientFd() << " successfully closed ( •⌄• ू )✧" << DEFAULT << std::endl;
			close((*it)->getClientFd());
			delete *it;
			it = clients.erase(it);
		}
		else
			++it;
	}
}

void parsingHeaders(Client * & client, char *buf, size_t &count, ssize_t & ret) {
	char *tmp;

	client->getReceivedData()->addData(buf, ret);

	if (client->getReceivedData()->findMemoryFragment(doubleCRLF, 4) != NOTFOUND) {
		client->setPhase(parsBody);
		client->getReceivedData()->addData((char *)"", 1);
		tmp = client->getReceivedData()->toPointer();
		client->setRequest(new Request(tmp));
		free(tmp);
		std::cout << LMAGENTA << "Request №" << count << " received successfully (◍•ᴗ•◍)❤" << DEFAULT << std::endl;
		count = count + 1;
		if (client->getRequest()->isHeadersParsed())
			client->setPhase(responseGenerate);
	}
}

void parsingBody(Client * & client, char *buf, ssize_t & ret) {
	size_t			len;

	client->getRequest()->getReqBody().addData(buf, ret);

	if (client->getRequest()->getTransferEncoding() != "chunked" &&
		client->getRequest()->getReqBody().getDataSize() >= (size_t)ft_atoi(client->getRequest()->getContentLength().c_str())) {
		client->getRequest()->getReqBody().cutData(atoi(client->getRequest()->getContentLength().c_str()));
		client->setPhase(responseGenerate);
	}

	if (client->getRequest()->getTransferEncoding() == "chunked" &&
		client->getRequest()->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != NOTFOUND) {
		len = client->getRequest()->getReqBody().findMemoryFragment("0\r\n\r\n", 5);
		client->getRequest()->getReqBody().cutData(len);
		client->getRequest()->ChunkedBodyProcessing();
		client->setPhase(responseGenerate);
	}
}

void sendResponse(Client * & client, ssize_t & ret) {
	char *			tmp;
	tmp = client->getToSendData()->toPointer();
	ret = write(client->getClientFd(), tmp + client->getSendBytes(), client->getToSendData()->getDataSize() - client->getSendBytes());
	if (ret < 0 || errno == EPIPE)
		exit(errno);
	client->setSendBytes(client->getSendBytes() + ret);
	free(tmp);
	if (client->getSendBytes() == client->getToSendData()->getDataSize()) {
		std::cout << GREEN << "Response №" << g_count - 1 << " successfully send ♡〜٩( ˃́▿˂̀ )۶〜♡" << DEFAULT << std::endl;
		if (client->getRequest()->getConnection() == "close")
			client->setPhase(closing);
		else
			client->clear();
	}
}

void responseGenerating(Client * & client) {
	char *tmp;

	client->setResponse(new Response());
	tmp = client->getResponse()->responseGenerating(*(client->getRequest()), client->getWebServer());
	client->getToSendData()->addData(tmp, client->getResponse()->getLenOfResponse());
	client->setPhase(sendingResponse);
	free(tmp);
}

void requestProcessing(std::list<Client *> & clients, fd_set & readSet, fd_set & writeSet) {
	char 				buf[BUFSIZE + 1];
	ssize_t				ret;

	for (std::list<Client*>::iterator i = clients.begin(); i != clients.end();) {
		if (FD_ISSET((*i)->getClientFd(), &readSet)) {
			ret = recv((*i)->getClientFd(), buf, BUFSIZE, 0);
			if (ret <= 0)
				(*i)->setPhase(closing);
			else if ((*i)->getPhase() < parsHeaders)
				(*i)->setPhase(parsHeaders);
			if ((*i)->getPhase() == parsHeaders)
				parsingHeaders(*i, buf, g_count, ret);
			else if ((*i)->getPhase() == parsBody)
				parsingBody(*i, buf, ret);
			if ((*i)->getPhase() == responseGenerate)
				responseGenerating(*i);
		}
		if (FD_ISSET((*i)->getClientFd(), &writeSet) && (*i)->getPhase() == sendingResponse)
			sendResponse(*i, ret);
		++i;
	}
}

void start_servers(std::vector<WebServer> & servers) {
	std::list<Client *> clients;
	int  				maxFd;
	fd_set				writeSet;
	fd_set				readSet;
	struct timeval 		t;

	std::cout << GREEN << "Webserver is started" << DEFAULT<< std::endl;
	signal(SIGPIPE, SIG_IGN);
	while (g_count) {
		fd_init(servers, writeSet, readSet, maxFd);

		settingToFdSets(clients, maxFd, writeSet, readSet);

		t.tv_sec = 360;
		t.tv_usec = 0;

		select(maxFd + 1, &readSet, &writeSet, 0, &t);

		checkNewClients(servers, readSet, clients);

		requestProcessing(clients, readSet, writeSet);

		closingConnections(clients);
	}
}