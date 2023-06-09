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
						ClientRequest request(strage[fd], findServerConfig(fd, strage[fd])); //TODO:findServerConfig() 作る
						strage[fd].erase();
						ServerResponse res = ServerResponse(request);
						sendResponse(fd, &masterSendFds, res.getResponse());  //sendする
						// sendResponse(fd, &masterRecvFds, &masterSendFds, "HTTP/1.1 200 OK\r\n\r\n");
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
		return ;
	_acceptedSockets[acceptedFd] = _sockets[fd];
	fcntl(acceptedFd, F_SETFL, O_NONBLOCK);
	FD_SET(acceptedFd, masterRecvFds);
	if (_maxFd < acceptedFd)
		_maxFd = acceptedFd;
}

void Webserv::recvRequest(int fd, fd_set *masterRecvFds, fd_set *masterSendFds, map<int ,string> &strage)
{
	char buffer[BUFSIZE + 1];
	int len;
	static map<int, int> checkedSize;

	len = recv(fd, buffer, BUFSIZE, 0);
	if (len == -1)
	{
		perror("recv");
		FD_CLR(fd, masterRecvFds);
		close(fd);
		return ;
	}
	else if (len == 0)
		return ;
	buffer[len] = '\0';
	strage[fd] += buffer;
	// strage[fd] += ('\0' + 26); //TODO:最後だけ加える??
	const unsigned long pos = strage[fd].find("\r\n\r\n");
	if (pos == string::npos)
		return ;
	ClientRequest req(strage[fd]);
	if (!req.getIsContent() && req.getTransferEncoding() != "chunked")
	{
		FD_CLR(fd, masterRecvFds);
		FD_SET(fd, masterSendFds);
		return ;
	}
	if (req.getIsContent())
	{
		if (strage[fd].size() >= stoul(req.getContentLength()) + pos + 4)
		{
			strage[fd] = strage[fd].substr(0, stoul(req.getContentLength()) + pos + 4);
			FD_CLR(fd, masterRecvFds);
			FD_SET(fd, masterSendFds);
			return ;
		}
	}
	if (req.getTransferEncoding() == "chunked") //TODO:まだ
	{
		string temp;
		temp.assign(strage[fd].substr(max(checkedSize[fd], 0)));
		if (temp.find("\r\n0\r\n") != string::npos)
		{
			FD_CLR(fd, masterRecvFds);
			FD_SET(fd, masterSendFds);
			checkedSize[fd] = 0;
			return ;
		}
		int size = temp.size();
		checkedSize[fd] += max(size - 10, 0);
	}
}

void Webserv::sendResponse(int fd, fd_set *masterSendFds, string res)
{
	int len;

	len = send(fd, res.c_str(), res.size(), 0);
	if (len == -1 || len == 0)
		perror("send");
	FD_CLR(fd, masterSendFds);
	close(fd);
}

string Webserv::getServerName(string& request)
{
	std::istringstream iss(request);
	string line;
	string tmp;
	string value;
	string serverName;

	getline(iss, line);
	getline(iss, line);
	std::istringstream iss_second_line(line);
	iss_second_line >> tmp >> value;
	serverName = value.substr(0, value.find(":"));
	return serverName;
}


ServerConfig Webserv::findServerConfig(int fd, string& request)
{
	for (unsigned long i = 0; i < _acceptedSockets[fd].size(); i++)
	{
		#ifdef DEBUG
			cout << _acceptedSockets[fd][i].getServerName() << endl;
		#endif
		if (_acceptedSockets[fd][i].getServerName() == getServerName(request))
			return _acceptedSockets[fd][i];
	}
	return _acceptedSockets[fd][0];
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
