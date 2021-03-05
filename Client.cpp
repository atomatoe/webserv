//
// Created by Wynn Elease on 3/4/21.
//

#include "Client.h"

/* get */

Request *Client::getRequest() { return _request; }

Response *Client::getResponse() {	return _response; }

Bytes *Client::getReceivedData() const { return _receivedData; }

Bytes *Client::getToSendData() const { return _toSendData; }

int Client::getPhase() const { return _phase; }

int Client::getServerFd() const { return _serverFd; }

int Client::getClientFd() const { return _clientFd; }

size_t Client::getSendBytes() const { return _sendBytes; }

/*constructor*/
Client::Client(WebServer & webServer, int & clientFd) {
	_webServer = webServer; //todo remove server from client
	_clientFd = clientFd;
	_serverFd = webServer.get_server_fd();
	_receivedData = new Bytes();
	_toSendData = new Bytes();
	_phase = parsHeaders;
	_sendBytes = 0;
	fcntl(_clientFd, F_SETFL, O_NONBLOCK);
}

Client::~Client() {
	close(_clientFd);
	//_receivedData->clear();
	//_toSendData->clear();
}

void Client::setRequest(Request *request) {
	_request = request;
}

void Client::setResponse(Response *response) {
	_response = response;
}

WebServer &Client::getWebServer() {
	return _webServer;
}

void Client::setPhase(int phase) {
	_phase = phase;
}


void Client::setSendBytes(size_t bytes){ _sendBytes = bytes;}
