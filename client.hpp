/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:21:28 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/03 13:24:37 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "includes.hpp"

class Client
{
    private:
        char *buff_read; // то что запросит клиент
        char *buff_write; // то что отдадим клиенту
    public:
        Client();
        ~Client() {};
};

#endif