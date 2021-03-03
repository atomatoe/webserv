//
// Created by Wynn Elease on 2/18/21.
//

#include <iostream>
#include "Bytes.h"

Bytes::Bytes() : _dataSize(0), _data(new std::list< std::pair<size_t, char *> >()), _charData(ft_strdup("")){}

Bytes::~Bytes() {}

void Bytes::addData(char *data, size_t size){
	_data->push_back(std::pair<size_t,char *>(size, ft_memdup(data, size)));
	_dataSize += size;
}

char *Bytes::toPointer() {
	size_t i = 0;
	char *ret = (char *)malloc(_dataSize);
	for (std::list< std::pair<size_t,  char *> >::iterator it = _data->begin(); it != _data->end(); ++it){
		memmove(ret + i, it->second, it->first);
		i += it->first;
	}
	return ret;
}

void Bytes::clear(){
	_data->clear();
	_dataSize = 0;
	free(_charData);
	_charData = NULL; //todo leak
}

//size_t Bytes::findMemoryFragment1(const char *toFind, size_t len) {
//	std::list< std::pair<size_t,  char *> >::iterator it = --(_data->end());
//	//std::cout << "++" << it->second << "++\n";
//	char *tmp = (char *)memmem(it->second, it->first, toFind, len);
//	if (tmp == NULL)
//		return (size_t)-1;
//	return _dataSize - it->first + (tmp - it->second);
//}

//size_t Bytes::find(const char &toFind, size_t len) {
//	std::list< std::pair<size_t,  char *> >::iterator it = _data->begin();
//	while (it != _data->end()) {
//		for (size_t i = )
//	}
//}

size_t Bytes::findMemoryFragment(char const *toFind, size_t len){
	char *data = toPointer();
	char *tmp = (char *)memmem(data, _dataSize, toFind, len);
	if (tmp == NULL)
		return (size_t)-1;
	return tmp - data;
}

Bytes Bytes::cutData(size_t len){
	Bytes ret = Bytes();
	if (len > _dataSize) {
		clear();
		return ret;
	}
	char *tmp = toPointer();
	_charData = (char *)malloc(len);

	memcpy(_charData, tmp, len); //todo own memcpy
	ret.addData(tmp + len, _dataSize - len);
	_data->clear();
	_dataSize = 0;
	addData(_charData, len);
	return ret;
}

Bytes & Bytes::operator=(const Bytes &bytes) {
	_data->clear();
	_data = bytes._data;
	_dataSize = bytes._dataSize;
	return *this;
}