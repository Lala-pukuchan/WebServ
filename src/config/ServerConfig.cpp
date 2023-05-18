/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:46:18 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/18 18:50:57 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "Servers.hpp"

ServerConfig::ServerConfig(void) : _serverName(""), _port(""), _alias(""), _maxBodySize(1), _autoindex(false)
{
}

ServerConfig::~ServerConfig(void)
{
}

ServerConfig& ServerConfig::operator=(const ServerConfig& src)
{
	if (this != &src)
	{
		this->_serverName = src._serverName;
		this->_port = src._port;
		this->_allowedMethods = src._allowedMethods;
		this->_maxBodySize = src._maxBodySize;
		this->_errorPage = src._errorPage;
		this->_locations = src._locations;
		this->_indexes = src._indexes;
		this->_autoindex = src._autoindex;
		this->_return_redirect = src._return_redirect;
		this->_upload_path = src._upload_path;
		this->_cgi_extension = src._cgi_extension;
	}
	return (*this);
}

std::map<std::string, LocationConfig> ServerConfig::getLocationConfigs() const
{
	return (_locations);
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
			LocationConfig location(*this);
			if (value.length() != 1 && value[value.size() - 1] == '/')
				value = value.substr(0, value.size() - 1);
			location.setLocationConfig(configStrings, pos, value);
			_locations[value] = location;
		}
		else if (key == "index")
			setIndex(line, pos);
		else if (key == "cgi_extension")
			setCgiExtension(line, pos);
		else if (key == "allow_methods")
			setAllowedMethods(line, pos);
		else if (key == "alias")
		{
			this->_alias = getOneValue(line, pos);
			if (this->_alias[this->_alias.size() - 1] == '/')
				this->_alias = this->_alias.substr(0, this->_alias.size() - 1);
		}
		else if (key == "server_name")
			this->_serverName = getOneValue(line, pos);
		else if (key == "listen")
			this->_port = getOneValue(line, pos);
		else if (key == "autoindex")
		{
			string autoindex = getOneValue(line, pos);
			if (autoindex == "on")
				this->_autoindex = true;
			else if (autoindex == "off")
				this->_autoindex = false;
			else
				throw ConfigContentError(pos, line);
		}
		else if (key == "upload_path")
			this->_upload_path = getOneValue(line, pos);
		else if (key == "client_max_body_size")
			this->_maxBodySize = std::stoi(getOneValue(line, pos));
		else if (key == "error_page" || key == "return")
			getIntValue(line);
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

void ServerConfig::getIntValue(const std::string &line)
{
	std::string		key;
	std::string		int_value;
	std::string		page;
	std::string		check;

	std::istringstream iss(line);
	iss >> key >> int_value >> page >> check;
	if (int_value.empty() || page.empty())
		throw EmptyValueError(0, line);
	if (int_value.find(';') != std::string::npos)
		throw ConfigContentError(0, line);
	if (page.find(';') != std::string::npos && !check.empty())
		throw SemicolonError(0, line);
	if (page.find(';') != std::string::npos)
		page = page.substr(0, page.find(';'));
	if (key == "error_page")
		_errorPage[std::atoi(int_value.c_str())] = page;
	else if (key == "return")
		_return_redirect[std::atoi(int_value.c_str())] = page;
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

void ServerConfig::setCgiExtension(const std::string &line, const size_t &pos)
{
	std::string key;
	std::istringstream iss(line);

	iss >> key;
	while(1)
	{
		std::string value;
		iss >> value;
		if (value.empty())
			throw SemicolonError(pos, line);
		if (value.find(';') == 0)
			break;
		if (value.find(';') != std::string::npos)
		{
			value = value.substr(0, value.find(';'));
			_cgi_extension.push_back(value);
			break;
		}
		_cgi_extension.push_back(value);
	}
	if (_cgi_extension.empty())
		throw EmptyValueError(pos, line);
}

void ServerConfig::setAllowedMethods(const std::string &line, const size_t &pos)
{
	std::string key;
	std::istringstream iss(line);

	iss >> key;
	while(1)
	{
		std::string value;
		iss >> value;
		if (value.empty())
			throw SemicolonError(pos, line);
		if (value.find(';') == 0)
			break;
		if (value.find(';') != std::string::npos)
		{
			value = value.substr(0, value.find(';'));
			if (value != "GET" && value != "POST" && value != "DELETE" && value != "PUT")
				throw InvalidMethodError(pos, line);
			_allowedMethods.push_back(value);
			break;
		}
		_allowedMethods.push_back(value);
	}
	if (_allowedMethods.empty())
		throw EmptyValueError(pos, line);
}

std::string ServerConfig::getPortString(void) const
{
	return (this->_port);
}

void ServerConfig::PrintServerConfig()
{
	std::cout << "server_name: " << this->_serverName << std::endl;
	std::cout << "port: " << this->_port << std::endl;
	std::cout << "upload_path: " << this->_upload_path << std::endl;
	std::cout << "index: ";
	for (size_t i = 0; i < this->_indexes.size(); i++)
		std::cout << this->_indexes[i] << " ";
	std::cout << std::endl;
	std::cout << "cgi_extension: ";
	for (size_t i = 0; i < this->_cgi_extension.size(); i++)
		std::cout << this->_cgi_extension[i] << " ";
	std::cout << std::endl;
	std::cout << "allowed_methods: ";
	for (size_t i = 0; i < this->_allowedMethods.size(); i++)
		std::cout << this->_allowedMethods[i] << " ";
	std::cout << std::endl;
	std::cout << "location: " << std::endl;
	std::cout << "error_page: " << std::endl;
	for (std::map<int, std::string>::iterator it = this->_errorPage.begin(); it != this->_errorPage.end(); it++)
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	std::cout << "return: " << std::endl;
	for (std::map<int, std::string>::iterator it = this->_return_redirect.begin(); it != this->_return_redirect.end(); it++)
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	std::cout << "-----" << std::endl;
	for (std::map<std::string, LocationConfig>::iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		std::cout << "  " << it->first << ": " << std::endl;
		it->second.PrintLocationConfig();
		std::cout << "-----" << std::endl;
	}
}

std::string ServerConfig::getServerName() const { return (this->_serverName); }

std::vector<std::string> ServerConfig::getAllowedMethods() const { return (this->_allowedMethods); }

int ServerConfig::getMaxBodySize() const { return (this->_maxBodySize); }

std::vector<std::string> ServerConfig::getIndexes() const { return (this->_indexes); }

std::map<int, std::string> ServerConfig::getErrorPage() const { return (this->_errorPage); }

std::map<int, std::string> ServerConfig::getReturnRedirect() const { return (this->_return_redirect); }

std::string ServerConfig::getUploadPath() const { return (this->_upload_path); }

bool ServerConfig::getAutoindex() const { return (this->_autoindex); }

std::vector<std::string> ServerConfig::getCgiExtension() const { return (this->_cgi_extension); }
