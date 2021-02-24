/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 18:11:47 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/18 16:11:02 by atomatoe         ###   ########.fr       */
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
	char *tmp = ft_strdup("\0");
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

	_reqString = ft_strdup(reqString);
	_pathToCgi = ft_strdup("");
	fillMap();
	parsRequest(reqString);
	tmp = strchr(_info["uri"], '?');
	_queryString = tmp ? ft_strdup(tmp + 1) : ft_strdup("");
}


Request::~Request(){
	 //todo free all needing
}


int Request::parsHeaders(char **strings){
	for (int i = 0; i < 18; i++) {
		_info[_headers[i]] = ft_strdup("");
		for (int j = 0; strings[j]; j++){
			if (strnstr(strings[j], _headers[i].c_str(), _headers[i].length()) != NULL){
				free(_info[_headers[i]]);
				_info[_headers[i]] = strdup(strings[j] + _headers[i].length() + 2); //todo leak
			}
		}
	}
	for (int i = 0; strings[i]; ++i) {
		free(strings[i]);
	}
	return 0;
}

void Request::ChunkedBodyProcessing(){
	size_t ind = 1;
	size_t size = 0;

	memBody *chunkBody = ft_memsplit(_reqBody.toPointer(), (char *)"\r\n", _reqBody.getDataSize(), 2);

	for (memBody:: iterator i = chunkBody->begin(); i->first != 0; ++i){
		std::cout << i->first << " " << i->second << std::endl;
	}

	for (memBody:: iterator i = chunkBody->begin(); i->first != 0; ++i){
		if (ind % 2)
			size = hexToDec(std::string(i->second));
		else {
			_chunkedReqBody.addData(i->second, size);
		}
		ind++;
	}
	_chunkedReqBody.addData((char *)"", 1);
	std::cout << _chunkedReqBody.getDataSize() << std::endl;
	//std::cout << "||" << _chunkedReqBody.toPointer() << "||" << std::endl;
}

int Request::parsRequest(char *reqString){
	char **strings;
	char *copy;
	copy = reqString;
  //  _headValid.valid(reqString);
	parsFirstLine(&copy); //todo valid
	std::map<std::string, char*>::iterator iter = _info.begin();
	if ((strings = ft_splitTim(copy, '\r')) == NULL)
		return -1;
	parsHeaders(strings); //todo valid
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
		//free(tmp);  //todo leak
		*copy = *copy + i + 1;
	}
	if ((tmp = strchr(*copy, ' ')) != NULL){
		i = tmp - *copy;
		tmp = ft_substr(*copy, 0, i); // сделал start с 1 символа (пропуская /)
		iter = _info.insert(iter, std::pair<std::string, char *>(std::string("uri"), tmp));
		//free(tmp); //todo leak
		*copy = *copy + i + 1;
	}
	if ((tmp = strchr(*copy, '\n')) != NULL){
		i = tmp - *copy;
		tmp = ft_substr(*copy, 0, i - 1);
		iter = _info.insert(iter, std::pair<std::string, char *>(std::string("http"), tmp));
		//free(tmp);  //todo leak
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
