//
// Created by Wynn Elease on 2/18/21.
//

#include "Bytes.hpp"

Bytes::Bytes() : _data(new std::list< std::pair<size_t, char *> >()), _dataSize(0) { }

Bytes::~Bytes() {
    clear();
    delete _data;
}

void Bytes::addData(char *data, size_t size){
    char *tmp = ft_memdup(data, size);
    _data->push_back(std::pair<size_t,char *>(size, tmp));
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

size_t Bytes::findMemoryFragmentEnd(const char *toFind, size_t len) {
	std::list< std::pair<size_t, char *> > :: iterator  i = (--_data->end());
	if (i->first >= len) {
		if (std::string(toFind) == std::string(i->second, i->first - len, i->first)) {
			return _dataSize - len;
		}
	}
	return findMemoryFragment(toFind, len);
}

void Bytes::clear() {
    for (std::list< std::pair<size_t,  char *> >::iterator i = _data->begin(); i != _data->end(); ++i)
        free(i->second);
    _data->clear();
    _dataSize = 0;
}

size_t Bytes::findMemoryFragment(char const *toFind, size_t len){
    size_t ret;
    char *data = toPointer();
    char *tmp = (char *)memmem(data, _dataSize, toFind, len);
    if (tmp == NULL) {
		free(data);
		return (size_t) -1;
	}
    ret = tmp - data;
	free(data);
    return ret;
}

Bytes Bytes::cutData(size_t len){
    Bytes ret = Bytes();
    if (len > _dataSize) {
        clear();
        return ret;
    }
    char *tmp = toPointer();

    ret.addData(tmp + len, _dataSize - len);
    clear();
    _dataSize = 0;
    addData(tmp, len);
    free(tmp);
    return ret;
}

Bytes & Bytes::operator=(const Bytes &bytes) {
    clear();
    delete _data;
    _data = bytes._data;
    _dataSize = bytes._dataSize;
    return *this;
}
