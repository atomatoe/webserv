#include "Request.hpp"

std::string _headers[18] = {
		"Accept-Charsets",
		"Accept-Language",
		"Allow",
		"Authorization",
		"Content-Language",
		"Content-Length",
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

Request::Request(char *reqString){
	_reqString = ft_strdup(reqString);
	fillMap();
	parsRequest(reqString);
}

Request::~Request(){}

int Request::parsHeaders(char **strings){
	for (int i = 0; i < 18; i++){
		for (int j = 0; strings[j]; j++){
			if (strnstr(strings[j], _headers[i].c_str(), _headers[i].length()) != NULL) {
				_info[_headers[i]] = strings[j] + _headers[i].length() + 2;
			}
		}
	}
	// for (std::map<std::string, char *>::iterator iter = _info.begin(); iter != _info.end(); iter++){
	// 	std::cout << iter->first << ": " << iter->second << std::endl;
	// }
	for (int i = 0; strings[i]; ++i){
		free(strings[i]);
	}
	return 0;
}

int Request::parsRequest(char *reqString){
	char **strings;
	char *copy;

	copy = reqString;
	parsFirstLine(&copy); //todo valid
	std::map<std::string, char*>::iterator iter = _info.begin();
	if ((strings = ft_split(copy, '\n')) == NULL)
		return -1;
	parsHeaders(strings); //todo valid
	return -1;
}

int Request::parsFirstLine(char **copy){
	int		i;
	char	*tmp;

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
		tmp = ft_substr(*copy, 0, i);
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