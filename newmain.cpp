//
// Created by Wynn Elease on 2/22/21.
//

#include "WebServ.hpp"
#include "./Request_Response/Request.hpp"
#include "./Request_Response/Response.hpp"
//#include "includes/includes.hpp"
#include "parse/ParseConfig.hpp"
#include "Client.hpp"

void fd_init(std::vector<WebServer> &servers, fd_set &fd_write, fd_set &fd_read, int &max_fd)
{
	FD_ZERO(&fd_write); // зануление fd
	FD_ZERO(&fd_read); // зануление fd
	for(std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET(it->get_server_fd(), &fd_read);
		max_fd = it->get_server_fd();
		//		FD_ZERO(&it->getFdRead());
		//		FD_ZERO(&it->getFdWrite());
		//FD_SET(it->get_server_fd(), &it->getFdWrite());
	}
}

void endOfReadingRequest(Client client, fd_set fd_write, fd_set fd_read){
	close(client.getClientFd());
	client.getReceivedData()->clear();
	client.getToSendData()->clear();
	FD_CLR(client.getClientFd(), &fd_read);
	FD_CLR(client.getClientFd(), &fd_write);
}

void init_servers(std::vector<WebServer> & servers, size_t count_server)
{
	size_t i = 0;
	while(i != count_server) {
		servers[i].create_socket();
		servers[i].build_server();
		i++;
	}
}

void start_servers(std::vector<WebServer> servers)
{
	std::vector<Client> clients;
	int max_fd;
	fd_set fd_write;
	fd_set fd_read;
	char *buf;
	int fd;
	Bytes tmp;
	size_t len;
	int ret;
	ssize_t ret2;
	if (!(buf = (char *) malloc(sizeof(char) * 2000001)))
		exit(1);

	int i = 1;
	while (i)
	{
		if (i++ == 0)
			break;
		fd_init(servers, fd_write, fd_read, max_fd);

		for (size_t i = 0; i != clients.size(); ++i){
			if (clients[i].getIsHeadersEnded() < 2)
				FD_SET(clients[i].getServerFd(), &fd_read);
			else
				FD_SET(clients[i].getServerFd(), &fd_write);
			if (clients[i].getServerFd() > max_fd)
				max_fd = clients[i].getServerFd();
		}

		select(max_fd + 1, &fd_read, &fd_write, NULL, NULL);

		for (std::vector<WebServer>::iterator it = servers.begin(); it != servers.end(); ++it) {
			if (FD_ISSET(it->get_server_fd(), &fd_read)) {
				std::cout << "in Accept: " << it->get_server_fd() << std::endl;
				fd = accept(it->get_server_fd(), 0, 0);
				if (fd > 0)
					clients.push_back(Client(*it, fd));
			}
		}

		for (size_t i = 0; i != clients.size(); ++i){
			if (FD_ISSET(clients[i].getServerFd(), &fd_read)) {
				ret = recv(clients[i].getServerFd(), buf, 2000000, 0);
				if (!clients[i].getIsHeadersEnded() && ret > 0)
				{
					clients[i].getReceivedData()->addData(buf, ret);
					if ((len = clients[i].getReceivedData()->findMemoryFragment(doubleCRLF, 4)) != (size_t) -1)
					{
						tmp = clients[i].getReceivedData()->cutData(len + 4);
						clients[i].setRequest(new Request(clients[i].getReceivedData()->toPointer()));
						clients[i].getRequest()->getReqBody().addData(tmp.toPointer(), tmp.getDataSize());
						clients[i].setIsHeadersEnded(1);
						ret = recv(clients[i].getClientFd(), buf, 0, 0);
						if (ret == 0) {
							clients[i].setIsHeadersEnded(2);
							goto tmp;
						}
					}
				}
				else if (ret > 0 && clients[i].getIsHeadersEnded() == 1) {
					clients[i].getRequest()->setReqBody(buf, ret);
					if (strncmp(clients[i].getRequest()->getTransferEncoding(), "chunked", 7) == 0 && (len = clients[i].getRequest()->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != (size_t) -1)) {
						clients[i].getRequest()->getReqBody().cutData(len + 5);
						clients[i].getRequest()->ChunkedBodyProcessing();
						clients[i].setIsHeadersEnded(2);
						endOfReadingRequest(clients[i], fd_write, fd_read);
						goto tmp;
					}
					else if (ret < 2000000 || clients[i].getRequest()->getReqBody().findMemoryFragment(doubleCRLF, 4) != (size_t) -1) {
						clients[i].getRequest()->getReqBody().addData((char *) "", 1);
						clients[i].setIsHeadersEnded(2);
						endOfReadingRequest(clients[i], fd_write, fd_read);
					}
				}
				if (ret == 0) {
					endOfReadingRequest(clients[i], fd_write, fd_read);
					break;
				}
			}
			tmp:
			if (clients[i].getIsHeadersEnded() == 2 && (FD_ISSET(clients[i].getClientFd(), &fd_read) || FD_ISSET(clients[i].getClientFd(), &fd_write)))
			{
				clients[i].setResponse(new Response());
				clients[i].getToSendData()->addData(clients[i].getResponse()->give_me_response(*clients[i].getRequest(), clients[i].getWebServer()), clients[i].getResponse()->getLenOfResponse());
				ret2 = 0;
				while (ret2 != clients[i].getToSendData()->getDataSize())
					ret2 += write(clients[i].getClientFd(), clients[i].getToSendData()->toPointer() + ret2, clients[i].getToSendData()->getDataSize() - ret2);
				clients.erase(clients.begin() + i);
			}
		}

	}
}

int main(int ac, char **av)
{
	ParseConfig parseConfig((ac == 2) ? av[1] : "configs/atomatoe/default.conf");
	std::vector<WebServer> servers =  parseConfig.parse();
	std::map<std::string, std::string> error_page;
	size_t count_server = servers.size();

	init_servers(servers, count_server);
	start_servers(servers);
	return(0);
}