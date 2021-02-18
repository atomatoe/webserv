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
#include <sstream>
#include <iomanip>

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
//char *Request::putChunkedReqBody(){
// std::cout << "HERE\n";
// std::vector<char> chunkedBody;
// char *buf = (char *)malloc(5000);
// size_t    size;
// char *tmp;
// int ret;
// do {
//    do {
//       ret = recv(_fd, buf, 5000, 0);
//       buf[ret] = '\0';
//    } while (strchr(buf, '\n'));
//    lseek(_fd, )
//    size = hexToDec(std::string(buf));
//    std::cout << size << std::endl;
//    tmp++;
//    for (size_t i = 0; i < size; ++i){
//       chunkedBody.push_back(*copy);
//       copy++;
//    }
// } while (!strnstr(buf, "\r\n", ret));
// return buf;
//}

Request::Request(char *reqString, int fd){
   char *tmp;
   _reqString = ft_strdup(reqString);
   _fd = fd;
   fillMap();
   parsRequest(reqString);
   _reqBody = ft_strdup(!(tmp = strnstr(reqString, "\r\n\r\n", strlen(reqString))) ? "" : tmp + 4);
   // std::cout << "~" << _reqBody << "~" << std::endl;
   // if (!strcmp(this->getTransferEncoding(), "chunked"))
   //    _reqBody = putChunkedReqBody();
   //std::cout << "|" << _reqBody << "|" << std::endl;
}
Request::~Request(){}
int Request::parsHeaders(char **strings){
   for (int i = 0; i < 18; i++){
      _info[_headers[i]] = ft_strdup("");
      for (int j = 0; strings[j]; j++){ //todo to search just in headers, without body
         if (strnstr(strings[j], _headers[i].c_str(), _headers[i].length()) != NULL){
            free(_info[_headers[i]]);
            _info[_headers[i]] = strdup(strings[j] + _headers[i].length() + 2); //todo leak
         }
      }
   }
   //for (std::map<std::string, char *>::iterator iter = _info.begin(); iter != _info.end(); iter++){
   // std::cout << iter->first << ": " << "|" << iter->second << "|" << std::endl;
   //}
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
   if ((strings = ft_split(copy, '\r')) == NULL)
      return -1;
   parsHeaders(strings); //todo valid
   return -1;
}
int Request::parsFirstLine(char **copy){
   int       i;
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
      tmp = ft_substr(*copy, 0, i); // сделал start с 1 символа (пропуская /)  ИСПРАВИЛ ОБРАТНО! 18 февраля
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