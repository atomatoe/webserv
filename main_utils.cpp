//
// Created by Quaro Tamarillo on 2/23/21.
//

#include "includes/includes.hpp"

void    errorMalloc() {
    std::cerr << "Malloc error" << std::endl;
    exit(1);
}

void	exitSignal(int signalNum) {
    (void)signalNum;
    std::cout << GREEN << "Webserv has finished working" << DEFAULT<< std::endl;
    exit(0);
}

void init_servers(std::vector<WebServer> & servers, size_t count_server)
{
    size_t i = 0;
    while(i != count_server)
    {
        servers[i].create_socket();
        servers[i].build_server();
        i++;
    }
}



//void WebServer::handle_requests(Client *client, fd_set& read_fd, fd_set& write_fd) throw(){
//	HTTPRequest*	request = client->getRequest();
//	char*			buf = (char*)calloc(4097, 1);
//	int				read_bytes;
//	int				size_buffer = 4097;
//
//	if (client->getStage() == parsing_request) {
//		read_bytes = recv(client->getSocket(), buf, size_buffer, 0);
//		buf[read_bytes] = 0;
//		try {
//			if (read_bytes > 0) {
//				client->getRequest()->parse_request_http(buf, read_bytes);
//				if (client->getRequest()->getParsingStage() == 3 ||
//					client->getRequest()->getParsingStage() == 2)
//					client->setStage(generate_response);
//			}
//			else if (read_bytes == 0)
//				client->setStage(close_connection);
//		}
//		catch (const std::string& status_value) {
//			VirtualServer*	virtual_server = searchVirtualServer(client);
////			Location*		location = virtual_server->findLocation(client->getRequest());
//
//			client->getResponse()->setStatusCode(status_value);
//			client->getResponse()->generateResponse();
//			client->setResponseBuffer(client->getResponse()->getResponse(), client->getResponse()->getBodySize());
//			client->setStage(send_response);
//		}
//	}
//	else if (client->getStage() == generate_response) {
//		treatmentStageGenerate(client);
//	}
//	else if (client->getStage() == send_response) {
//		if (client->getBytes() != client->getSendBytes()) {
//			if (client->getSocket() < getMaxFd()) {
////				FD_CLR(client->getSocket(), &read_fd);
////				FD_CLR(client->getSocket(), &write_fd);
//				client->setStage(close_connection);
//				return;
//			}
//			size_t ret;
//			std::cout << "SEND BYRES: " << client->getSendBytes() << " " << client->getBytes() << std::endl;
//			if (FD_ISSET(client->getSocket(), &write_fd)) {
//				std::cout << client->getSocket() << std::endl;
//				ret = send(client->getSocket(), client->getReponseBuffer() + client->getSendBytes(),
//						   client->getBytes() - client->getSendBytes(), 0);
//				client->setSendBytes(client->getSendBytes() + ret);
//				if (client->getSendBytes() > client->getBytes()) {
//					client->setSendBytes(0);
//				}
//				return;
//			}
//		}
//		FD_CLR(client->getSocket(), &read_fd);
//		FD_CLR(client->getSocket(), &write_fd);
//		client->setStage(close_connection);
//	}
//}
//
