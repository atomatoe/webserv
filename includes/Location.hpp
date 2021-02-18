/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:02:35 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/18 13:11:11 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "includes.hpp"

class Location {
private:
	std::string _URI;
	std::string _root;
	bool 		_autoIndex;
	std::string _cgiPath;
	size_t 		_limitBody;
	std::map<std::string, bool> _allowMethods;
	std::string _index;
public:
	Location(std::string uri, std::string root, bool autoIndex, std::string cgiPath, std::map<std::string, bool> allowMethods, std::string index);
	~Location();
	// Timurrrr 4ast костыли
	std::string get_index() { return(_index); }
	std::map<std::string, bool> get_method() { return(_allowMethods); }
	std::string get_uri() { return(_URI); }
	std::string get_root() { return(_root); }
	bool get_autoindex() { return(_autoIndex); }
};

#endif