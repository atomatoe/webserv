/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 13:26:03 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/02 16:08:02 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h> // для socket
#include <netinet/in.h> // для sockaddr_in
#include <arpa/inet.h> // для inet_addr
// #include <sys/types.h>

// http://netcode.ru/cpp/?lang=&katID=19&skatID=176&artID=5155
// htonl, htons, ntohl, ntohs - переводят данные из узлового порядка расположения байтов в сетевой и наоборот   - нужно будет реализовать свою 
// 127.0.0.1 - взять 127 перевести в двоичный систему исчисление, также и 0,0,1. 
// Затем склеить в двоичном виде каждый элемент (нельзя нарушать порядок) unsigned int addr = 127 << 24 | 0 << 16 | 0 << 8 | 1 // либо использовать inet_addr
// затем addr >> 24 | addr << 24; - получается htonl 
// https://coderoad.ru/39460263/%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5-htonl-%D0%B4%D0%BB%D1%8F-%D0%BF%D1%80%D0%B0%D0%B2%D0%B8%D0%BB%D1%8C%D0%BD%D0%BE%D0%B3%D0%BE-%D0%BF%D1%80%D0%B5%D0%BE%D0%B1%D1%80%D0%B0%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F-localhost-%D1%81-%D0%BF%D0%BE%D0%BC%D0%BE%D1%89%D1%8C%D1%8E-C
int main(int argc, char **argv)
{
    // if(argc != 2)
    //     std::cout << "Неверное количество аргументов" << std::endl;
    // else if(ft_parser_config_file() == 0)
    //     return (-1);
    //
    // Создаем сокет
    // printf("%ld\n", (long)(127 << 24 | 0 << 16 | 0 << 8 | 1));
    // int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    // sockaddr_in test;
    // test.sin_family = AF_INET;
    // unsigned int addr = 2130706433;
    // test.sin_addr.s_addr = addr >> 24 | addr << 24; // содержит адрес (номер) узла сети.
    // std::cout << test.sin_addr.s_addr << std::endl;
    // test.sin_port = 8080;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in test;
    socklen_t address_len = (sizeof(test));
    test.sin_family = AF_INET;
    test.sin_addr.s_addr = inet_addr("127.0.0.1"); // содержит адрес (номер) узла сети.
    test.sin_port = htons(8086); // 80 - port с конфига  что делает htons (80 >> 8 | 80 << 8)
    int status = bind (server_fd, (const struct sockaddr *) &test, address_len); // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
    std::cout << "status: " << status << std::endl;
    int yes = 1; // не знаю что это но это работает
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)); // дает возможность повторно использовать сокет (повторять bind)
    
    return (0);
}



/*
struct sockaddr_in 
{ 
    __uint8_t         sin_len;  // длина в байтах структуры
    sa_family_t       sin_family;  // флаг передачи между сокетами (интернет/локалка)
    in_port_t         sin_port;  // порт
    struct in_addr    sin_addr;  // структура в которой лежит ip adress
    char              sin_zero[8]; 
};
*/