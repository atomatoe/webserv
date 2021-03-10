//
// Created by Wynn Elease on 3/6/21.
//

#include "../Includes/Includes.hpp"

void	exitSignal(int signalNum) {
	(void)signalNum;
	std::cout << GREEN << "Webserv has finished working" << DEFAULT << std::endl;
	exit(0);
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

int main(int ac, char **av) {
	signal(SIGINT, exitSignal);
	signal(SIGTERM, exitSignal);
	signal(SIGQUIT, exitSignal);

	ParseConfig parseConfig((ac == 2) ? av[1] : "WebServSite/website.conf");
	std::vector<WebServer> servers =  parseConfig.parse();
	size_t count_server = servers.size();

	init_servers(servers, count_server);
	start_servers(servers);
	return(0);
}
