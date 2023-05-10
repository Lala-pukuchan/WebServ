#include "Webserv.hpp"

Webserv::Webserv(Servers &servers) : _servs(servers) {}

Webserv::~Webserv() {}

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

	// for debug
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