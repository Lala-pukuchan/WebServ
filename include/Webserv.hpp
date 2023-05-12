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
		static struct timeval _timeout;
		int _maxFd;
		map<int, vector<ServerConfig> > _sockets;
		map<int, vector<ServerConfig>& > _acceptedSockets;

		void setFds(fd_set *masterRecvFds);
		bool matchListenFd(int fd);
		void makeAcceptedFd(int fd, fd_set *masterRecvFds);
		void recvRequest(int fd, fd_set *masterRecvFds, fd_set *masterSendFds, map<int ,string> &strage);
		void sendResponse(int fd, fd_set *masterRecvFds, fd_set *masterSendFds, map<int, string> &strage);

		void printdebug();

	public:
		Webserv(Servers& servers);
		~Webserv();

		void makeServerSocket(void);
		void run(void);
};

#endif