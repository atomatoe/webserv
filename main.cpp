#include "WebServ.hpp"
#include "./Request_Response/Request.hpp"
#include "./Request_Response/Response.hpp"
//#include "includes/includes.hpp"
#include "parse/ParseConfig.hpp"

void fd_init(std::vector<WebServer> &servers, fd_set &fd_write_tmp, fd_set &fd_read_tmp, fd_set &fd_write, fd_set &fd_read, int &max_fd)
{
	max_fd = 0;// потом нужно найти самый большой fdшник и передевать его первым аргументом в select.
	FD_ZERO(&fd_write); // зануление fd
	FD_ZERO(&fd_read); // зануление fd
	FD_ZERO(&fd_write_tmp); // зануление fd
	FD_ZERO(&fd_read_tmp); // зануление fd
	for(size_t it = 0; it != servers.size(); it++)
	{
		FD_SET(servers[it].get_server_fd(), &fd_read); // заносим fd сокета(то есть сервера) в список fdшников
		FD_SET(servers[it].get_server_fd(), &fd_write); // заносим fd сокета(то есть сервера) в список fdшников
		max_fd = servers[it].get_server_fd();
	}
}

//void accepting(std::vector<WebServer> &servers, size_t it, fd_set &fd_write, fd_set &fd_read, int &max_fd) {
//	int fd = accept(servers[it].get_server_fd(), (struct sockaddr *) servers[it].get_out(), servers[it].get_address_len()); // fd - новый клиент
//	if(fd > 0) {
//		FD_SET(fd, &fd_read); // заносим клиента в fd для чтения
//		FD_SET(fd, &fd_write); // заносим клиента в fd для записи
//		if (fd > max_fd)
//			max_fd = fd;
//		(servers[it].get_list())[fd].receivedData = new Bytes();
//		(servers[it].get_list())[fd].toSendData = new Bytes();
//		(servers[it].get_list())[fd].isHeadersEnded = 0;
//	}
//}



void endOfReadingRequest(std::map<int, t_client>::iterator it2, fd_set fd_write, fd_set fd_read,
						 WebServer server){
	close(it2->first);
	it2->second.receivedData->clear();
	it2->second.toSendData->clear();
	FD_CLR(it2->first, &fd_read);
	FD_CLR(it2->first, &fd_write);
	//server.get_list().erase(it2);
}

void start_servers(std::vector<WebServer> servers) {
	int 					max_fd;
	fd_set                  fd_write;
	fd_set                  fd_read;
	fd_set                  fd_write_tmp; // fd_write_tmp - нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
	fd_set                  fd_read_tmp; // fd_read_tmp - нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
	fd_init(servers, fd_write_tmp, fd_read_tmp, fd_write, fd_read, max_fd);
	size_t i = 1;
	char* newbuf = (char *)malloc(sizeof(char) * 2000001);
	int i1 = 0;
	while(1)
	{
		i1++;
		if (i++ == 0)
			break;
		for(size_t it = 0; it != servers.size(); it++)
		{
			fd_write_tmp = fd_write;
			fd_read_tmp = fd_read;
			select(max_fd + 1, &fd_read_tmp, &fd_write_tmp, NULL, NULL);
			if(FD_ISSET(servers[it].get_server_fd(), &fd_read_tmp)) { // 1 блок = проверка на пришел кто то или нет
				//std::cout << "HERE\n";
				int fd = accept(servers[it].get_server_fd(), 0, 0); // fd - новый клиент
				std::cout << "TEST" << std::endl;
				if(fd >= 0) {
					FD_SET(fd, &fd_read); // заносим клиента в fd для чтения
					FD_SET(fd, &fd_write); // заносим клиента в fd для записи
					if (fd > max_fd)
						max_fd = fd;
					(servers[it].get_list())[fd].receivedData = new Bytes();
					(servers[it].get_list())[fd].toSendData = new Bytes();
					(servers[it].get_list())[fd].isHeadersEnded = 0;
				}
				//accepting(servers, it, fd_write, fd_read, max_fd);
			}
			for(std::map<int, t_client>::iterator it2 = (servers[it].get_list()).begin(); it2 != (servers[it].get_list()).end(); it2++) // 2 блок = на получение данных (отправил или нет)
			{
				Bytes tmp;
				if(FD_ISSET(it2->first, &fd_read_tmp))
				{
					size_t len;
					int ret = recv(it2->first, newbuf, 2000000, 0);
					if (!it2->second.isHeadersEnded && ret > 0){
						//std::cout << "|||" << newbuf << "|||" << std::endl;
						it2->second.receivedData->addData(newbuf, ret);
						//std::cout << "---" << it2->second.receivedData->toPointer() << "---" << std::endl;
						if ((len = it2->second.receivedData->findMemoryFragment(doubleCRLF, 4)) != (size_t)-1){
							tmp = it2->second.receivedData->cutData(len + 4);
							it2->second.request = new Request(it2->second.receivedData->toPointer());
							std::cout << "after req: " << i1 << std::endl;
							it2->second.request->getReqBody().addData(tmp.toPointer(), tmp.getDataSize());
							it2->second.isHeadersEnded = 1;
							ret = recv(it2->first, newbuf , 0, 0);
							if (ret == 0){
								it2->second.isHeadersEnded = 2;
								//it2->second.toSendData->addData(response.give_me_response(*request, servers[it]), response.getLenOfResponse());
								//break;
								goto tmp;
							}
						}
					}
					else if (ret > 0 && it2->second.isHeadersEnded == 1){
						it2->second.request->setReqBody(newbuf, ret);
						if (strncmp(it2->second.request->getTransferEncoding(), "chunked", 7) == 0 && (len = it2->second.request->getReqBody().findMemoryFragment("0\r\n\r\n", 5) != (size_t)-1)){
							it2->second.request->getReqBody().cutData(len + 5);
							it2->second.request->ChunkedBodyProcessing();
							it2->second.isHeadersEnded = 2;
							endOfReadingRequest(it2, fd_write, fd_read, servers[it]);
							//it2->second.toSendData->addData(response.give_me_response(*request, servers[it]), response.getLenOfResponse());
						}
						else if (ret < 2000000 || it2->second.request->getReqBody().findMemoryFragment(doubleCRLF, 4) != (size_t)-1){
							//it2->second.toSendData->addData(response.give_me_response(*request, servers[it]), response.getLenOfResponse());
							it2->second.request->getReqBody().addData((char *)"", 1);
							it2->second.isHeadersEnded = 2;
							endOfReadingRequest(it2, fd_write, fd_read, servers[it]);
						}
					}
					if (ret == 0){
						endOfReadingRequest(it2, fd_write, fd_read, servers[it]);
						break ;
					}
				}
				tmp:
				if(it2->second.isHeadersEnded == 2) {
					it2->second.response = new Response();
					std::cout << "before res: " << i1 << std::endl;
					it2->second.toSendData->addData(it2->second.response->give_me_response(*(it2->second.request), servers[it]), it2->second.response->getLenOfResponse());
					ssize_t ret = write(it2->first, it2->second.toSendData->toPointer(), it2->second.toSendData->getDataSize());
					//std::cout << "BBBBBB: " << it2->second.toSendData->getDataSize() << std::endl;
					//std::cout << "CCCCCC: " << ret << std::endl;
					if(ret != it2->second.toSendData->getDataSize()) {
						ret = send(it2->first, it2->second.toSendData->toPointer() + ret, it2->second.toSendData->getDataSize() - ret, 0);
						std::cout << "AAAAAAAAAA\n";
						char *buf = NULL;
						buf = ft_memjoin(newbuf, it2->second.toSendData->toPointer(), 0,it2->second.toSendData->getDataSize() + ret); //todo maybe important thing
						it2->second.toSendData->clear();
						it2->second.toSendData->addData(buf, strlen(buf)); //todo maybe too important
						close(it2->first);
					}
					else {
						close(it2->first); // как отправили все данные клиенту, отсоединяемся
						it2->second.toSendData->clear();
					}
					it2->second.isHeadersEnded = 0;
					delete it2->second.response;
					delete it2->second.request;
					it2->second.receivedData->clear();
					it2->second.toSendData->clear();
					FD_CLR(it2->first, &fd_read_tmp);
					FD_CLR(it2->first, &fd_write_tmp);
				}
			}
		}
	}
}

void init_servers(std::vector<WebServer> & servers, size_t count_server)
{
	size_t i = 0;
	while(i != count_server)
	{
		servers[i].create_socket();
		servers[i].build_server();
		i++;
	}
}

int main(int ac, char **av)
{
	ParseConfig parseConfig((ac == 2) ? av[1] : "configs/atomatoe/default.conf");
	std::vector<WebServer> servers =  parseConfig.parse();
	std::map<std::string, std::string> error_page;
	size_t count_server = servers.size();

	init_servers(servers, count_server);
	start_servers(servers);
	return(0);
}
