//
// Created by Wynn Elease on 2/28/21.
//

#include "libft/libft.h"
#include <iostream>

int main() {
	int tmp = 0;
	char *tmp1 = ft_memdup("c8\r\ncccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\r\n0\r\n\r\n", 219);
	memBody *res = ft_memsplit(tmp1, (char *)"\r\n", 219, 2);
	for (memBody:: iterator i = res->begin(); i != res->end(); ++i){
		std::cout << tmp++ <<" in chunk: " << i->first << " |" << i->second  << "|" << std::endl;
	}
}