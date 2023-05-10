#ifndef Webserv_HPP
# define Webserv_HPP

# include "ServerConfig.hpp"
# include "Servers.hpp"
# include "ServerSocket.hpp"
using namespace std;

class Webserv
{
	private:
		Servers _servs;
		map<int, vector<ServerConfig> > _sockets;
		map<int, vector<ServerConfig>& > acceptedSockets;

		void printdebug();

	public:
		Webserv(Servers& servers);
		~Webserv();

		void makeServerSocket(void);
};

#endif