/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 13:26:03 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/03 13:30:42 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h> // для socket
#include <netinet/in.h> // для sockaddr_in
#include <arpa/inet.h> // для inet_addr
#include <fcntl.h> // для fcntl
#include <unistd.h> // для close, write, open и другой хуеты
#include <sys/select.h> // для макросов с работой select FD_CLR, FD_ISSET, FD_SET
#include <map> // угадай для чего
#include <stdlib.h> // для маллок
// #include <sys/types.h>

// http://netcode.ru/cpp/?lang=&katID=19&skatID=176&artID=5155
// htonl, htons, ntohl, ntohs - переводят данные из узлового порядка расположения байтов в сетевой и наоборот   - нужно будет реализовать свою 
// 127.0.0.1 - взять 127 перевести в двоичный систему исчисление, также и 0,0,1. 
// Затем склеить в двоичном виде каждый элемент (нельзя нарушать порядок) unsigned int addr = 127 << 24 | 0 << 16 | 0 << 8 | 1 // либо использовать inet_addr
// затем addr >> 24 | addr << 24; - получается htonl 
// https://coderoad.ru/39460263/%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5-htonl-%D0%B4%D0%BB%D1%8F-%D0%BF%D1%80%D0%B0%D0%B2%D0%B8%D0%BB%D1%8C%D0%BD%D0%BE%D0%B3%D0%BE-%D0%BF%D1%80%D0%B5%D0%BE%D0%B1%D1%80%D0%B0%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F-localhost-%D1%81-%D0%BF%D0%BE%D0%BC%D0%BE%D1%89%D1%8C%D1%8E-C

// nc -c 127.0.0.1 8086  - отсылает вместе с \r\n (без флага -c только с \n)
// \r\n -- так присылают 
typedef struct client 
{ 
    char *buff_read; // то что запросит клиент
    char *buff_write; // то что отдадим клиенту
} t_client;

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len; 
	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = (char *)malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

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


    fd_set fd_read, fd_read_tmp, fd_write, fd_write_tmp; // здесь будут хранится все fd наших клиентов и основной fd где открыт сервер
    // fd_read_tmp -- нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
    FD_ZERO(&fd_write); // зануление списка fd для записи
    FD_ZERO(&fd_read); // зануление списка fd для чтения
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in test;
    socklen_t address_len = (sizeof(test));
    test.sin_family = AF_INET;
    test.sin_addr.s_addr = inet_addr("127.0.0.1"); // содержит адрес (номер) узла сети.
    test.sin_port = htons(8080); // 80 - port с конфига  что делает htons (80 >> 8 | 80 << 8)
    int yes = 1; // не знаю что это но это работает
    int status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)); // дает возможность повторно использовать сокет (повторять bind)
    if(status != 0)
        std::cerr << "setsockopt FAILED" << std::endl;
    status = bind (server_fd, (const struct sockaddr *) &test, address_len); // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
    if(status != 0)
        std::cerr << "bind FAILED" << std::endl;
    // status = fcntl(server_fd, F_SETFL, O_NONBLOCK); // делаем сокет неблокирующемся
    if(status != 0)
        std::cerr << "fcntl FAILED" << std::endl;
    status = listen(server_fd, 0);
    if(status != 0)
        std::cerr << "listen FAILED" << std::endl;
    sockaddr_in out;
    // после listen не нужна проверка status
    // int fd = accept(server_fd, (struct sockaddr *) &out, &address_len); // out - заполнит значениями клиента - пример принятия запроса
    // fd - новый клиент
    // nc 127.0.0.1 8086 для себя сохранил, команда терминала
    // select, accept и recv send в бесконечном цикле
    FD_SET(server_fd, &fd_read); // заносим fd сокета(то есть сервера) в список fdшников
    FD_SET(server_fd, &fd_write); // заносим fd сокета(то есть сервера) в список fdшников
    int max_fd = server_fd; // потом нужно найти самый большой fdшник и передевать его первым аргументом в select.
    std::map<int, t_client> list;
    while(1)
    {
        fd_read_tmp = fd_read;
        fd_write_tmp = fd_write;
        select(max_fd + 1, &fd_read_tmp, &fd_write_tmp, NULL, NULL);
        // 1 блок = проверка на пришел кто то или нет 
        // 2 блок = на получение данных (отправил или нет) 
        // 3 блок = готов ли кто-то получить от тебя данные или нет
        if(FD_ISSET(server_fd, &fd_read_tmp)) // fd_read_tmp - засорится из за проверки, поэтому нужна tmp // 1 блок
        {
            int fd = accept(server_fd, (struct sockaddr *) &out, &address_len); // out - заполнит значениями клиента - пример принятия запроса
            if(fd > 0)
            {
                FD_SET(fd, &fd_read); // заносим клиента в fd для чтения
                FD_SET(fd, &fd_write); // заносим клиента в fd для записи
                if(fd > max_fd)
                    max_fd = fd;
                list[fd].buff_read = 0;
                list[fd].buff_write = 0;
            }
            // send(fd, "hello world!\n", 20, 0); // отвечаем клиенту

        }
        for(std::map<int, t_client>::iterator it = list.begin(); it != list.end(); it++) // 2 блок на получение данных
        {
            if(FD_ISSET(it->first, &fd_read_tmp))
            {
                char* newbuf = (char *)malloc(sizeof(char) * 4097);
                int ret = recv(it->first, newbuf, 4096, 0);
                if(ret == 0)
                {
                    close(it->first);
                    if(it->second.buff_read)
                        free(it->second.buff_read);
                    if(it->second.buff_write)
                        free(it->second.buff_write);
                    FD_CLR(it->first, &fd_read);
                    FD_CLR(it->first, &fd_write);
                    list.erase(it);
                    free(newbuf);
                    break ; // делаем break чтобы не наебнулась it++ 
                }
                else if(ret > 0)
                {
                    newbuf[ret] = '\0';
                    it->second.buff_read = str_join(it->second.buff_read, newbuf);
                    // после str_join нужно сделать проверку на сообщение на наличие \r\n (это означает конец передаваемого сообщение). 
                    // например: 123\r\n 456\r\n 789\r\n  - берем 123, отправляем на обработку, оставляем только 456\r\n789\r\n.
                    std::cout << it->second.buff_read << std::endl;
                    it->second.buff_write = str_join(it->second.buff_write, "Ваше сообщение получено.\nОтвет: ты хуйло.\n"); 
                }
                free(newbuf);
            }
        }
        for(std::map<int, t_client>::iterator it = list.begin(); it != list.end(); it++) // 3 блок готов ли кто-то получить от тебя данные или нет
        {
            if(FD_ISSET(it->first, &fd_write_tmp))
            {
                if(it->second.buff_write)
                {
                    int fd = open("index.html", O_RDONLY);
                    char buff[5000];
                    char* hello = (char *)malloc(sizeof(char) * 4097);
                    int i = read(fd, hello, 5555);
                    // char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                    write(it->first , hello , strlen(hello));
                    int ret = send(it->first, it->second.buff_write, strlen(it->second.buff_write), 0);
                    if(ret != strlen(it->second.buff_write))
                    {
                        char *newbuf = NULL;
                        newbuf = str_join(newbuf, it->second.buff_write + ret);
                        free(it->second.buff_write);
                        it->second.buff_write = newbuf;
                    }
                    else
                    {
                        free(it->second.buff_write);
                        it->second.buff_write = NULL;
                    }
                }
            }
        }
    }




   
    // close(server_fd); // необязательно делать, система должна автоматически сделать
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