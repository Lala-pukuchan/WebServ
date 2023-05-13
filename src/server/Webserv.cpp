#include "Webserv.hpp"

// #define DEBUG

#define BUFSIZE 1024
struct timeval Webserv::_timeout = {0, 200};

Webserv::Webserv(Servers &servers) : _servs(servers) {}

Webserv::~Webserv() {}

//
//public
//
void Webserv::makeServerSocket(void)
{
	map<string, vector<ServerConfig> >::iterator it = _servs.getServers().begin();

	for (; it != _servs.getServers().end(); ++it)
	{
		ServerSocket socket(it->first);
		_sockets[socket.getFd()] = it->second;
		if (_maxFd < socket.getFd())
			_maxFd = socket.getFd();
	}
	printdebug();
}

void Webserv::run(void)
{
	fd_set	masterRecvFds;
	fd_set	masterSendFds;
	fd_set	recvFds; // accept(接続要求), recv（受け取り）
	fd_set	sendFds; // send
	map<int, string>	strage;

	FD_ZERO(&masterRecvFds);
	FD_ZERO(&masterSendFds);
	setFds(&masterRecvFds);

	while(true)
	{
		FD_ZERO(&recvFds);
		FD_ZERO(&sendFds);
		memcpy(&recvFds, &masterRecvFds, sizeof(masterRecvFds));
		memcpy(&sendFds, &masterSendFds, sizeof(masterSendFds));
		
		switch (select(_maxFd + 1, &recvFds, &sendFds, NULL, &_timeout))
		{
			case -1:
				perror("select");
				break;
			case 0:
				break;
			default:
				for (int fd = 0; fd < _maxFd + 1; fd++)
				{
					if (FD_ISSET(fd, &recvFds))
						if (_sockets.find(fd) != _sockets.end()) // 接続が確立されるsocketsの方に含まれるなら，
							makeAcceptedFd(fd, &masterRecvFds); //acceptする
						else
							recvRequest(fd, &masterRecvFds, &masterSendFds, strage); //recvする
					else if (FD_ISSET(fd, &sendFds))
					{
						sendResponse(fd, &masterRecvFds, &masterSendFds, strage);  //sendする
					}
				}
		}
	}
}


//
//private
//

void Webserv::setFds(fd_set *masterRecvFds)
{
	map<int, vector<ServerConfig> > ::iterator it = _sockets.begin();

	for (; it != _sockets.end(); it++)
	{
		FD_SET(it->first, masterRecvFds);
		#ifdef DEBUG
		cout << "port is " << it->first << endl;
		#endif
	}
}

void Webserv::makeAcceptedFd(int fd, fd_set *masterRecvFds)
{
	int acceptedFd;

	acceptedFd = accept(fd, NULL, NULL);
	if (acceptedFd == -1)
	{
	 	if (errno != EWOULDBLOCK) //ノンブロッキングでacceptedFdが-1になっているときは，エラーにならない
			perror("accept");
		return ;
	}
	// _acceptedSockets[acceptedFd] = _sockets[fd];
	fcntl(acceptedFd, F_SETFL, O_NONBLOCK);
	FD_SET(acceptedFd, masterRecvFds);
	if (_maxFd < acceptedFd)
		_maxFd = acceptedFd;
}

void Webserv::recvRequest(int fd, fd_set *masterRecvFds, fd_set *masterSendFds, map<int ,string> &strage)
{
	char buffer[BUFSIZE + 1];
	int len;

	len = recv(fd, buffer, BUFSIZE, 0);
	if (len == -1)
		perror("recv");
	strage[fd] += buffer;
	if (strage[fd][BUFSIZE - 1] == 0) // end of file
	{
		FD_CLR(fd, masterRecvFds);
		FD_SET(fd, masterSendFds);
		#ifdef DEBUG
		cout << "buffer is " << strage[fd] << endl;
		#endif
	}
}

void Webserv::sendResponse(int fd, fd_set *masterRecvFds, fd_set *masterSendFds, map<int, string> &strage)
{
	int len;

	len = send(fd, strage[fd].c_str(), strage[fd].size(), 0);
	if (len == -1)
		perror("send");
	strage[fd].erase();
	FD_CLR(fd, masterSendFds);
	close(fd);
	(void) masterRecvFds;
}

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

// bool Webserv::matchListenFd(int fd)
// {
// 	if (_sockets.find(fd) == _sockets.end())
// 		return false;
// 	return true;
// }