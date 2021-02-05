/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:47:33 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/05 19:30:36 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

#include "includes.hpp"

class Autoindex
{
    private:
        char* _code;
    public:
        Autoindex(char *directory)
        {
            // buffer = str_join(buffer, directory);
            chdir(directory); // переходим в папку root
        
            char* buffer = getcwd(NULL, 0);
            if (buffer == NULL)
                perror("getcwd error");
            std::cout << buffer << std::endl;
            
        }
        ~Autoindex() { };
};

#endif