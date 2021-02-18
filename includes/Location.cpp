/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:02:37 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/18 12:19:55 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::string uri, std::string root, bool autoIndex, std::string cgiPath, std::map<std::string, bool> allowMethods, std::string index)
{
    this->_URI = uri;
    this->_root = root;
    this->_autoIndex = autoIndex;
    this->_cgiPath = cgiPath;
    this->_allowMethods = allowMethods;
    this->_index = index;
}

Location::~Location() { }