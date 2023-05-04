/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:46:18 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/04 23:43:21 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "Servers.hpp"

ServerConfig::ServerConfig(void) : _port("")
{
}

ServerConfig::~ServerConfig(void)
{
}

void ServerConfig::setServerConfig(const std::vector<std::string> &configStrings, size_t &pos)
{
	std::string line;

	while (pos < configStrings.size())
	{
		line = configStrings[pos];
		std::istringstream iss(line);
		std::string	key;
		std::string	value;
		std::string check;
		iss >> key;
		pos++;
		if (key.empty())
			continue;
		else if (key == "}")
			break;
		else if (key == "location")
		{
			iss >> value >> check;
			if (value.empty() || check != "{")
				throw EmptyValueError(pos, line);
			LocationConfig location;
			location.setLocationConfig(configStrings, pos);
			_locations[value] = location;
		}
		else if (key == "index")
			setIndex(line, pos);
		else if (key == "server_name")
			this->_serverName = getOneValue(line, pos);
		else if (key == "listen")
			this->_port = getOneValue(line, pos);
		else
			throw InvalidKeyError(pos, line);
	}
	if (this->_port.empty())
		throw EmptyPortError();
}

std::string ServerConfig::getOneValue(const std::string &line, size_t &pos)
{
	std::string key;
	std::string value;
	std::string check;

	std::istringstream iss(line);
	iss >> key >> value >> check;
	if (value.empty() ||value.find(';') == 0)
		throw EmptyValueError(pos, line);
	if ((value.find(';') == std::string::npos && check.find(';') != 0) || (value.find(';') != std::string::npos && check.find(';') == 0))
		throw SemicolonError(pos, line);
	if (value.find(';') != std::string::npos)
		value = value.substr(0, value.find(';'));
	return (value);
}

void ServerConfig::setIndex(const std::string &line, const size_t &pos)
{
	std::string key;
	std::istringstream iss(line);

	iss >> key;
	while(1)
	{
		std::string value;
		iss >> value;
		if (value.empty())
			throw EmptyValueError(pos, line);
		if (value.find(';') == 0)
			break;
		if (value.find(';') != std::string::npos)
		{
			value = value.substr(0, value.find(';'));
			_indexes.push_back(value);
			break;
		}
		_indexes.push_back(value);
	}
	if (_indexes.empty())
		throw EmptyValueError(pos, line);
}

std::string ServerConfig::getPortString(void)
{
	return (this->_port);
}
