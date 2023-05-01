/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:02:19 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/01 17:25:14 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "Servers.hpp"

Servers::Servers()
{
}

Servers::~Servers()
{
}

void Servers::ReadConfig(std::string path)
{
	std::ifstream	ifs(path);
	std::string		line;
	size_t			comment_pos;
	
	if (!ifs)
		throw FileOpenError();
	while (getline(ifs, line))
	{
		comment_pos = line.find('#');
		if (comment_pos != std::string::npos)
			line = line.substr(0, comment_pos);
		this->_configStrings.push_back(line);
	}
	ifs.close();
	if (this->_configStrings.size() == 0)
		throw FileEmptyError();
}

void Servers::setServersConfig(void)
{
	std::string		line;
	std::string	split_line[2];
	size_t			pos = 0;

	while (pos < this->_configStrings.size())
	{
		if (_configStrings[pos].empty())
		{
			pos++;
			continue ;
		}
		std::istringstream iss(_configStrings[pos]);
		iss >> split_line[0] >> split_line[1] >> split_line[2];
		if (!split_line[2].empty())
			throw ConfigContentError(pos, _configStrings[pos]);
		else if (split_line[0] == "server" && split_line[1] == "{" \
			|| split_line[0] == "server{" && split_line[1].empty())
		{
			ServerConfig server;
			server.setServerConfig(_configStrings, ++pos);
			this->_servers[server.getPortString()].push_back(server);
		}
		else
			throw ConfigContentError(pos, _configStrings[pos]);
	}
}

// for debug
void Servers::PrintConfig()
{
	std::vector<std::string>::iterator it = this->_configStrings.begin();
	std::vector<std::string>::iterator ite = this->_configStrings.end();
	
	while (it != ite)
	{
		std::cout << *it << std::endl;
		it++;
	}
}