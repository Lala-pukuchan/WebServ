/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:02:19 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/10 21:41:53 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

Servers::Servers() : _defaultServerFlag(false)
{
}

Servers::~Servers()
{
}

ServerConfig Servers::getDefaultServer()
{
	return (this->_defaultServer);
}

void Servers::ReadConfig(string path)
{
	ifstream	ifs(path);
	string		line;
	size_t			comment_pos;
	
	if (!ifs)
		throw FileOpenError();
	while (getline(ifs, line))
	{
		comment_pos = line.find('#');
		if (comment_pos != string::npos)
			line = line.substr(0, comment_pos);
		this->_configStrings.push_back(line);
	}
	ifs.close();
	if (this->_configStrings.size() == 0)
		throw FileEmptyError();
}

void Servers::setServersConfig(void)
{
	string		line;
	string	split_line[3];
	size_t			pos = 0;

	while (pos < this->_configStrings.size())
	{
		if (_configStrings[pos].empty())
		{
			pos++;
			continue ;
		}
		istringstream iss(_configStrings[pos]);
		iss >> split_line[0] >> split_line[1] >> split_line[2];
		if (!split_line[2].empty())
			throw ConfigContentError(pos, _configStrings[pos]);
		else if ((split_line[0] == "server" && split_line[1] == "{") \
			|| split_line[0] == "server{")
		{
			ServerConfig server;
			server.setServerConfig(_configStrings, ++pos);
			this->_servers[server.getPortString()].push_back(server);
			if (_defaultServerFlag == false)
			{
				this->_defaultServer = server;
				_defaultServerFlag = true;
			}
		}
		else
			throw ConfigContentError(pos, _configStrings[pos]);
	}
}

// for debug
void Servers::PrintConfigFile()
{
	vector<string>::iterator it = this->_configStrings.begin();
	vector<string>::iterator ite = this->_configStrings.end();
	
	while (it != ite)
	{
		cout << *it << endl;
		it++;
	}
}

void Servers::PrintServersConfig()
{
	cout << "-------------------------------------" << endl;
	for (map<string, vector<ServerConfig> >::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		vector<ServerConfig> &server_configs = it->second;

		for (vector<ServerConfig>::iterator server_it = server_configs.begin(); server_it != server_configs.end(); ++server_it)
		{
			server_it->PrintServerConfig();
			cout << "-------------------------------------" << endl;
		}
	}
}

map<string, vector<ServerConfig> >& Servers::getServers()
{
	return _servers;
}