//
// Created by Wynn Elease on 3/4/21.
//

#ifndef WEBSERV_CLIENT_H
#define WEBSERV_CLIENT_H


#include "../Request/Request.hpp"

class Client {

private:
	Request		*_request;
	Response	*_response;
	Bytes 		*_receivedData;// то что запросит клиент
	Bytes		*_toSendData; // то что отдадим клиенту
	int			_phase;
	int 		_serverFd;
	int 		_clientFd;
	WebServer	_webServer;
	size_t		_sendBytes;
	time_t 		_time;

public:
	Client(WebServer & webServer, int & clientFd);
	~Client();

	/* get */
	Request		*getRequest();
	Response	*getResponse();
	Bytes		*getReceivedData() const;
	Bytes		*getToSendData() const;
	int			getPhase() const;
	int			getServerFd() const;
	int			getClientFd() const;
	WebServer & getWebServer();
	size_t		getSendBytes() const;
	time_t 		getTime() const;

	/* set */
	void 		setRequest(Request * request);
	void		setResponse(Response * response);
	void 		setSendBytes(size_t bytes);
	void 		setPhase(int phase);
	void 		clear();
};

enum Phase {
	start = 0,
	parsHeaders,
	parsBody,
	responseGenerate,
	sendingResponse,
	closing
};

#endif //WEBSERV_CLIENT_H
