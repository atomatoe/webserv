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
	_phase = start;
	_sendBytes = 0;
	_request = nullptr;
	_response = nullptr;
	fcntl(_clientFd, F_SETFL, O_NONBLOCK);
}

void Client:: clear() {
	delete _receivedData;
	delete _toSendData;
	_phase = start;
	_sendBytes = 0;

	delete _request;
	delete _response;
	_request = nullptr;
	_response = nullptr;

	_receivedData = new Bytes();
	_toSendData = new Bytes();
}

Client::~Client() {
	delete _receivedData;
	delete _toSendData;
	_phase = start;
	_sendBytes = 0;
	if (_phase >= parsHeaders)
		delete _request;
	if (_phase >= sendingResponse)
		delete _response;
}

void Client::setRequest(Request *request) {
	if (_request != nullptr)
		delete _request;
	_request = request;
}

/*set*/

void Client::setResponse(Response *response) { _response = response; }

WebServer &Client::getWebServer() { return _webServer; }

void Client::setPhase(int phase) { _phase = phase; }

void Client::setSendBytes(size_t bytes){ _sendBytes = bytes;}