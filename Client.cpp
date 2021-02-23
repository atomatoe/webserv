//
// Created by Wynn Elease on 2/23/21.
//

#include "Client.hpp"



/* get */

Request *Client::getRequest() { return _request; }

Response *Client::getResponse() {	return _response; }

Bytes *Client::getReceivedData() const { return _receivedData; }

Bytes *Client::getToSendData() const { return _toSendData; }

int Client::getIsHeadersEnded() const { return _isHeadersEnded; }

int Client::getServerFd() const { return _serverFd; }

int Client::getClientFd() const { return _clientFd; }

/*constructor*/
Client::Client(WebServer webServer, int clientFd) {
	_webServer = webServer; //todo remove server from client
	_clientFd = clientFd;
	_serverFd = webServer.get_server_fd();
	_receivedData = new Bytes();
	_toSendData = new Bytes();
	_isHeadersEnded = 0;
}

Client::~Client() {
	close(_clientFd);
	//_receivedData->clear();
	//_toSendData->clear();
}

void Client::setRequest(Request *request) {
	_request = request;
}

void Client::setIsHeadersEnded(int isHeadersEnded)
{
	_isHeadersEnded = isHeadersEnded;
}

void Client::setResponse(Response *response) {
	_response = response;
}

WebServer &Client::getWebServer() {
	return _webServer;
}
