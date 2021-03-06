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
	std::map<std::string, char*>::iterator iter = _info.begin();
	char *tmp = strdup("\0");
	while (i < 18){
		iter = _info.insert(iter, std::pair<std::string, char *>(std::string(_headers[i]), tmp));
		i++;
	}
	free(tmp);
}

std::string ToHex(const std::string& s, bool upper_case /* = true */)
{
	std::ostringstream ret;
	for (std::string::size_type i = 0; i < s.length(); ++i)
		ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];
	return ret.str();
}

size_t hexToDec(const std::string &s) { return strtoul(s.c_str(), NULL, 16); }


void Request::setReqBody(char *body, size_t len){
	_reqBody.addData(body, len);
}

Request::Request(char *reqString){
	char *tmp;
	size_t len;

	tmp = strstr(reqString, "\r\n\r\n");
	len = tmp - reqString + 4;
	_reqBody.addData(reqString + len, strlen(reqString) - len);
	_reqString = ft_substr(reqString, 0, len);
	_parsedHeaders = false;
	fillMap();
	parsRequest(reqString);
	std::cout << "after" << std::endl;
	tmp = strchr(_info["uri"], '?');
	_queryString = tmp ? tmp + 1 : "";
	if (strcmp(getMetod(), "GET") == 0 || strcmp(getMetod(), "HEAD") == 0)
		_parsedHeaders = true;
	else if (strcmp(getTransferEncoding() , "chunked") == 0 && _reqBody.findMemoryFragment("0\r\n\r\n", 5) != (size_t)-1) {
		len = _reqBody.findMemoryFragment("0\r\n\r\n", 5);
		_reqBody.cutData(len);
		ChunkedBodyProcessing();
		_parsedHeaders = true;
	}
	if (_reqBody.getDataSize() < atoi(getContentLength()) && strcmp(getContentLength(), "") != 0) {
		Bytes tmp = _reqBody.cutData(atoi(getContentLength()));
		tmp.clear();
		_parsedHeaders = true;
	}
}

bool Request::isHeadersParsed() { return _parsedHeaders; };

Request::~Request(){
}


int Request::parsHeaders(char **strings){
	for (int i = 0; i < 18; i++) {
		_info[_headers[i]] = strdup("");
		for (int j = 0; strings[j]; j++){
			if (strnstr(strings[j], _headers[i].c_str(), strlen(strings[j])) != NULL){
				free(_info[_headers[i]]);
				_info[_headers[i]] = strdup(strings[j] + _headers[i].length() + 3); //todo leak
			}
		}
	}
	for (int i = 0; strings[i]; ++i)
		free(strings[i]);
	free(strings);
	return 0;
}

void Request::ChunkedBodyProcessing(){
	size_t ind = 1;
	size_t size = 0;

	memBody *chunkBody = ft_memsplit(_reqBody.toPointer(), (char *)"\r\n", _reqBody.getDataSize(), 2);
	if (chunkBody->size() == 0)
		return;
	for (memBody:: iterator i = chunkBody->begin(); i != chunkBody->end(); ++i){
		if (ind % 2)
			size = hexToDec(std::string(i->second));
		else
			_chunkedReqBody.addData(i->second, size);
		ind++;
	}
	_reqBody.clear();
	_reqBody.addData(_chunkedReqBody.toPointer(), _chunkedReqBody.getDataSize());
}

int Request::parsRequest(char *reqString){
	char	**strings;
	char	*copy;
	char 	*t;

	copy = strdup(reqString);
	t = copy;
   // _headValid.valid(reqString); // todo try-catch exception
	parsFirstLine(&copy);
	std::map<std::string, char*>::iterator iter = _info.begin();
	if ((strings = ft_splitTim(copy, '\r')) == NULL)
		return -1;
	parsHeaders(strings);
	free(t);
	return -1;
}
int Request::parsFirstLine(char **copy){
	size_t       i;
	char   *tmp;
	std::map<std::string, char*>::iterator iter = _info.begin();
	if ((tmp = strchr(*copy, ' ')) != NULL){
		i = tmp - *copy;
		tmp = ft_substr(*copy, 0, i);
		iter = _info.insert(iter, std::pair<std::string, char *>(std::string("metod"), tmp));
		*copy = *copy + i + 1;
	}
	if ((tmp = strchr(*copy, ' ')) != NULL){
		i = tmp - *copy;
		tmp = ft_substr(*copy, 0, i); // сделал start с 1 символа (пропуская /)
		iter = _info.insert(iter, std::pair<std::string, char *>(std::string("uri"), tmp));
		*copy = *copy + i + 1;
	}
	if ((tmp = strchr(*copy, '\n')) != NULL){
		i = tmp - *copy;
		tmp = ft_substr(*copy, 0, i - 1);
		iter = _info.insert(iter, std::pair<std::string, char *>(std::string("http"), tmp));
		*copy = *copy + i + 1;
	}
	return 0;
}

std::string &Request::getPathToCgi(){
	return _pathToCgi;
}

void Request::setPathToCgi(const std::string &pathToCgi) {
	_pathToCgi = pathToCgi;
}

std::string &Request::getQueryString() {
	return _queryString;
}
