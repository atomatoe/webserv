/*
 * 1.hello.cpp
 * Copyright (C) 2015 uralbash <root@uralbash.ru>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>

using namespace std;

int main()
{
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>Hello World</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<h2>Hello World</h2>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return 0;
}