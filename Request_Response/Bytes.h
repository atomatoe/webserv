//
// Created by Wynn Elease on 2/18/21.
//

#ifndef WEBSERV21_BYTES_H
#define WEBSERV21_BYTES_H


#include <stdlib.h>
#include <list>
#include "../libft/libft.h"
class Bytes
{
private:
	std::list< std::pair<size_t, char *> >*_data;
	size_t _dataSize;
	char *_charData;
	bool _cutFlag;
public:
	Bytes();
	~Bytes();
	void addData(char *data, size_t size);
	size_t findMemoryFragment(char const *toFind, size_t len);
	size_t getDataSize() const {return _dataSize;}
	Bytes cutData(size_t len); //return ostatok ot cut
	bool isEmpty();
	char *toPointer();
	void clear();
};


#endif //WEBSERV21_BYTES_H
