//
// Created by Wynn Elease on 2/23/21.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP


#include <Request.hpp>

class Client {

private:
	Request		*_request;
	Response	*_response;
	Bytes 		*_receivedData;// то что запросит клиент
	Bytes		*_toSendData; // то что отдадим клиенту
	int			_isHeadersEnded;
	int 		_serverFd;
	int 		_clientFd;
	WebServer	_webServer;

public:
	Client(WebServer webServer, int clientFd);
	~Client();

	/* get */
	Request		*getRequest();
	Response	*getResponse();
	Bytes		*getReceivedData() const;
	Bytes		*getToSendData() const;
	int			getIsHeadersEnded() const;
	int			getServerFd() const;
	int			getClientFd() const;
	WebServer & getWebServer();

	/* set */
	void 		setRequest(Request * request);
	void		setResponse(Response * response);
	void 		setIsHeadersEnded(int isHeadersEnded);

};


#endif //WEBSERV_CLIENT_HPP
