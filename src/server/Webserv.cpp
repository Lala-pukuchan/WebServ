#include "Webserv.hpp"

// #define DEBUG

Webserv::Webserv(Servers &servers) : _servs(servers) {}

Webserv::~Webserv() {}

#ifdef DEBUG
void Webserv::printdebug()
{
	map<int, vector<ServerConfig> >::iterator soc;
	for (soc = _sockets.begin(); soc != _sockets.end(); ++soc)
	{
		cout << "\033[32;1m" << soc->first << "\033[0m" << endl;
		for (unsigned long i = 0; i < soc->second.size(); i++)
		{
			cout << "\033[31;1m" << i << "\033[0m" << endl;
			soc->second[i].PrintServerConfig();
		}
	}
}
#else
void Webserv::printdebug() {}
#endif

void Webserv::makeServerSocket(void)
{
	map<string, vector<ServerConfig> >::iterator it = _servs.getServers().begin();

	for (; it != _servs.getServers().end(); ++it)
	{
		ServerSocket socket(it->first);
		_sockets[socket.getFd()] = it->second;
		// for (unsigned long i = 0; i < it->second.size(); i++)
		// 	_sockets[socket.getFd()].push_back(it->second[i]);
	}
	printdebug();
}

// void Webserv::run(void)
// {
// 	fd_set	masterRecvFds;
// 	fd_set	masterSendFds;
// 	fd_set	recvFds;
// 	fd_set	sendFds;
// 	int		maxFd;
// 	int		readyNum;
// 	struct timeval	timeout;
// 	map<int, string>	fd_response;
// 	timeout.tv_sec  = 0;
// 	timeout.tv_usec = 100;

// 	maxFd = setFds(_sockets, &masterRecvFds);

// 	FD_ZERO(&masterSendFds);
// 	while(true)
// 	{
// 		memcpy(&recvFds, &masterRecvFds, sizeof(masterRecvFds));//
// 		memcpy(&sendFds, &masterSendFds, sizeof(masterSendFds));//FD_ZEROとFD_SETでいいかも
		
// 		readyNum = select(maxFd + 1, &recvFds, &sendFds, NULL, &timeout);
// 		if (readyNum == 0)
// 			continue ;
// 		else if (readyNum == -1)
// 			perror("select");
// 		else
// 		{
// 			for (int fd = 0; maxFd + 1; fd++)
// 			{
// 				if (FD_ISSET(fd, &recvFds))
// 				{
// 					if (containsListeningSocket(fd, _sockets)) // 接続が確立されるsocketsの方に含まれるなら，
// 						createClntSocket(fd, &masterRecvFds, &maxFd); //acceptする
// 					else
// 						storeRequestToMap(fd, &masterRecvFds, &masterSendFds, fd_response); //recvする
// 				}
// 				else if (FD_ISSET(fd, &sendFds))
// 				{
// 					sendResponse(fd, &masterRecvFds, &masterSendFds, &maxFd, fd_response);  //sendする
// 					std::cout << "clnt_socket: " <<  fd << ", maxFd: " << maxFd << std::endl;
// 				}
				
// 			}
// 		}
// 	}
// }