/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:46:18 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/01 19:08:02 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Servers.hpp"

ServerConfig::ServerConfig(void)
{
}

ServerConfig::~ServerConfig(void)
{
}

void ServerConfig::setServerConfig(std::vector<std::string> &configStrings, size_t &pos)
{
	std::string	key;
	std::string	value;
	std::string check;

	while (pos < configStrings.size())
	{
		if (configStrings[pos].empty())
		{
			pos++;
			continue;
		}
		std::istringstream iss(configStrings[pos]);
		iss >> key;
		if (key == "}")
			break;
		if (key == "location")
		{

		}
		if (key == "index")
		{
			iss >> value;
			if (value.empty() || isAllSemicolon(value))
				throw EmptyValueError(pos, configStrings[pos]);
			while(1)
			{
				if (isAllSemicolon(value))
					break;
				if (value.find(';') != std::string::npos && !isAllSemicolon(value + value.find(';')))
					value = value.substr(0, value.find(';'));

			}
		}
		iss >> value >> check;
		if (value.empty() ||value.find(';') == 0)
			throw EmptyValueError(pos, configStrings[pos]);
		if ((value.find(';') == std::string::npos && check.find(';') != 0) || (value.find(';') != std::string::npos && check.find(';') == 0))
			throw SemicolonError(pos, configStrings[pos]);
		if (value.find(';') != std::string::npos)
			value = value.substr(0, value.find(';'));
		value = value.substr(0, value.size() - 1);
		if (key == "server_name")
			this->_serverName = value;
		else if (key == "listen")
			this->_port = value;
	}
}
