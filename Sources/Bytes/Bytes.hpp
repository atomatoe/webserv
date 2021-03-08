//
// Created by Wynn Elease on 2/18/21.
//

#ifndef WEBSERV21_BYTES_H
#define WEBSERV21_BYTES_H


#include <stdlib.h>
#include <list>
#include "../../Libft/libft.h"
class Bytes
{
private:
	std::list< std::pair<size_t, char *> >*_data;
	size_t _dataSize;
	char *_charData;
public:
	Bytes();
	~Bytes();
	void addData(char *data, size_t size);
	size_t findMemoryFragment(char const *toFind, size_t len);
	//size_t findMemoryFragment1(const char *toFind, size_t len);
	size_t getDataSize() const {return _dataSize;}
	Bytes cutData(size_t len); //return remainder of cut
	//size_t find(const char &toFind, size_t len);
	char *toPointer();
	void clear();
	Bytes & operator=(Bytes const & bytes);
};


#endif //WEBSERV21_BYTES_H
