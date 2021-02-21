//
// Created by Wynn Elease on 2/18/21.
//

#include <iostream>
#include "Bytes.h"

Bytes::Bytes() : _dataSize(0), _cutFlag(false), _data(new std::list< std::pair<size_t, char *> >()), _charData(ft_strdup("")){}

Bytes::~Bytes() {}

void Bytes::addData(char *data, size_t size){
	//std::cout << "+++" << data << "+++";
	_data->push_back(std::pair<size_t,char *>(size, ft_memdup(data, size)));
	_dataSize += size;
}

char  *Bytes::toPointer(){
	if (_cutFlag)
		return _charData;
	char *tmp2;
	char *tmp1;
	size_t s = 0;
	tmp2 = ft_memdup("", 1);
	//std::list< std::pair<size_t,  char *> >::iterator it1 = _data->begin();
	for (std::list< std::pair<size_t,  char *> >::iterator it = _data->begin(); it != _data->end(); ++it){
		tmp1 = tmp2;
		tmp2 = ft_memjoin(tmp2, it->second, s, it->first);
		s += it->first;
		free(tmp1);
	}
	return tmp2;
}

void Bytes::clear(){
	_data->clear();
	_dataSize = 0;
	_cutFlag = false;
	_charData = NULL; //todo leak
}

bool Bytes::isEmpty(){
	return _dataSize == 0;
}

size_t Bytes::findMemoryFragment(char const *toFind, size_t len){
	char *data = toPointer();
	char *tmp = ft_memmem(data, _dataSize, toFind, len);
	if (tmp == NULL)
		return (size_t)-1;
	return tmp - data;
}

Bytes Bytes::cutData(size_t len){
	Bytes ret;
	char *tmp = toPointer();
	_charData = (char *)malloc(len + 1);

	memcpy(_charData, tmp, len); //todo own memcpy
	_charData[len] = '\0';
	ret.addData(tmp + len, _dataSize - len);
	_cutFlag = true;
	return ret;
}
