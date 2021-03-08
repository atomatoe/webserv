/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 18:11:47 by atomatoe          #+#    #+#             */
/*   Updated: 2021/03/03 15:51:57 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"


std::string _headers[18] = {
		"Accept-Charsets",
		"Accept-Language",
		"Allow",
		"Authorization",
		"Content-Language",
		"content-length",
		"Content-Location",
		"Content-Type",
		"Date",
		"Host",
		"Last-Modified",
		"Location",
		"Referer",
		"Retry-After",
		"Server",
		"Transfer-Encoding",
		"User-Agent",
		"WWW-Authenticate"
};

void Request::fillMap(){
	int i = 0;
	std::map<std::string, std::string>::iterator iter = _info.begin();
	while (i < 18){
		iter = _info.insert(iter, std::pair<std::string, std::string>(std::string(_headers[i]), ""));
		i++;
	}
}

size_t hexToDec(const std::string &s) { return strtoul(s.c_str(), NULL, 16); }

void Request::checkEndOfBody() {
	size_t 	len;
	if (getMetod() == "GET" || getMetod() == "HEAD")
		_parsedHeaders = true;
	if (getTransferEncoding() == "chunked" && _reqBody.findMemoryFragment("0\r\n\r\n", 5) != (size_t)-1) {
		len = _reqBody.findMemoryFragment("0\r\n\r\n", 5);
		_reqBody.cutData(len);
		ChunkedBodyProcessing();
		_parsedHeaders = true;
	}
	if (getTransferEncoding() != "chunked" && _reqBody.getDataSize() > atoi(getContentLength().c_str()) && getContentLength() != "") {
		Bytes tmp = _reqBody.cutData(stoi(getContentLength()));
		tmp.clear();
		_parsedHeaders = true;
	}
}

Request::Request(char *reqString){
	char 	*tmp;
	size_t 	len;

	tmp = strstr(reqString, "\r\n\r\n");
	len = tmp - reqString + 4;
	_reqBody.addData(reqString + len, strlen(reqString) - len);
	_reqString = ft_substr(reqString, 0, len);
	_parsedHeaders = false;
	fillMap();
	parsRequest(reqString);
	len = _info["uri"].find( '?', 0);
	_queryString = len != std::string::npos ? _info["uri"].c_str() + len + 1 : "";
	checkEndOfBody();
}

bool Request::isHeadersParsed() { return _parsedHeaders; };

Request::~Request(){
	free(_reqString);
}


int Request::parsHeaders(char **strings){
	for (int i = 0; i < 18; i++) {
		_info[_headers[i]] = "";
		for (int j = 0; strings[j]; j++){
			if (strnstr(strings[j], _headers[i].c_str(), strlen(strings[j])) != NULL){
				_info[_headers[i]] = strings[j] + _headers[i].length() + 3; //todo leak
			}
		}
	}
	for (int i = 0; strings[i]; ++i)
		free(strings[i]);
	free(strings);
	return 0;
}

void Request::ChunkedBodyProcessing(){
	size_t 		ind = 1;
	size_t 		size = 0;
	char *		tmp;

	tmp = _reqBody.toPointer();
	memBody *chunkBody = ft_memsplit(tmp, (char *)"\r\n", _reqBody.getDataSize(), 2);
	free(tmp);
	if (chunkBody->size() == 0) {
		delete chunkBody;
		return;
	}
	_chunkedReqBody.clear();
	for (memBody:: iterator i = chunkBody->begin(); i != chunkBody->end(); ++i){
		if (ind % 2)
			size = hexToDec(std::string(i->second, i->first));
		else
			_chunkedReqBody.addData(i->second, size);
		ind++;
	}
	_reqBody.clear();
	tmp = _chunkedReqBody.toPointer();
	_reqBody.addData(tmp, _chunkedReqBody.getDataSize());
	free(tmp);
	_chunkedReqBody.clear();
	for (memBody:: iterator i = chunkBody->begin(); i != chunkBody->end(); ++i)
		free(i->second);
	delete chunkBody;
}

int Request::parsRequest(char *reqString) {
	char	**strings;
	char	*copy;
	char 	*t;

	copy = strdup(reqString);
	t = copy;
   // _headValid.valid(reqString); // todo try-catch exception
	parsFirstLine(&copy);
	std::map<std::string, std::string>::iterator iter = _info.begin();
	if ((strings = ft_splitTim(copy, '\r')) == NULL)
		return -1;
	parsHeaders(strings);
	free(t);
	return -1;
}

int Request::parsFirstLine(char **copy){
	size_t       i;
	char   *tmp;
	std::map<std::string, std::string>::iterator iter = _info.begin();
	if ((tmp = strchr(*copy, ' ')) != NULL){
		i = tmp - *copy;
		iter = _info.insert(iter, std::pair<std::string, std::string>(std::string("metod"), std::string(*copy).substr(0, i)));
		*copy = *copy + i + 1;
	}
	if ((tmp = strchr(*copy, ' ')) != NULL){
		i = tmp - *copy;
		iter = _info.insert(iter, std::pair<std::string, std::string>(std::string("uri"), std::string(*copy).substr(0, i)));
		*copy = *copy + i + 1;
	}
	if ((tmp = strchr(*copy, '\n')) != NULL){
		i = tmp - *copy;
		_info.insert(iter, std::pair<std::string, std::string>(std::string("http"), std::string(*copy).substr(0, i - 1)));
		*copy = *copy + i + 1;
	}
	return 0;
}

std::string &Request::getPathToCgi(){ return _pathToCgi; }

void Request::setPathToCgi(const std::string &pathToCgi) { _pathToCgi = pathToCgi; }

std::string &Request::getQueryString() { return _queryString; }
