/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 16:13:34 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/13 18:56:03 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include "Servers.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

LocationConfig::LocationConfig(void)
{
}

LocationConfig::LocationConfig(const ServerConfig &serverConfig) : _allowedMethods(serverConfig.getAllowedMethods()), _maxBodySize(serverConfig.getMaxBodySize()), _errorPage(serverConfig.getErrorPage()), _indexes(serverConfig.getIndexes()), _autoindex(serverConfig.getAutoindex()), _upload_path(serverConfig.getUploadPath()), _cgi_extension(serverConfig.getCgiExtension()), _return_redirect(serverConfig.getReturnRedirect())
{
}

LocationConfig::~LocationConfig(void)
{
}


void LocationConfig::setLocationConfig(const std::vector<std::string> &configStrings, size_t &pos, const std::string &locationName)
{
	std::string		line;

	while (pos < configStrings.size())
	{
		line = configStrings[pos];
		std::istringstream iss(line);
		std::string		key;
		std::string		value;
		std::string		check;
		iss >> key;
		pos++;
		if (key.empty())
			continue;
		else if (key == "}")
			break;
		else if (key == "error_page" || key == "return")
			getIntValue(line);
		else if (key == "alias")
			this->_alias = getOneValue(line, pos);
		else if (key == "allow_methods")
			setAllowedMethods(line, pos);
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
		else if (key == "client_max_body_size")
			this->_maxBodySize = std::stoi(getOneValue(line, pos));
		else if (key == "upload_path")
			this->_upload_path = getOneValue(line, pos);
		else if (key == "cgi_extension")
			setCgiExtension(line, pos);
		else if (key == "cgi_path")
			this->_cgi_path = getOneValue(line, pos);
		else if (key == "index")
			setIndex(line, pos);
		else
			throw InvalidKeyError(pos, line);
	}
	if (locationName[0] != '*' && this->_alias.empty())
		this->_alias = locationName;
}

std::string LocationConfig::getOneValue(const std::string &line, size_t &pos)
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

void LocationConfig::setAllowedMethods(const std::string &line, const size_t &pos)
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

void LocationConfig::getIntValue(const std::string &line)
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

void LocationConfig::setIndex(const std::string &line, const size_t &pos)
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

void LocationConfig::setCgiExtension(const std::string &line, const size_t &pos)
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

void LocationConfig::PrintLocationConfig()
{
	std::cout << "alias: " << this->_alias << std::endl;
	std::cout << "autoindex: " << this->_autoindex << std::endl;
	std::cout << "upload_path: " << this->_upload_path << std::endl;
	std::cout << "cgi_extension: ";
	for (size_t i = 0; i < this->_cgi_extension.size(); i++)
		std::cout << this->_cgi_extension[i] << " ";
	std::cout << std::endl;
	std::cout << "index: ";
	for (size_t i = 0; i < this->_indexes.size(); i++)
		std::cout << this->_indexes[i] << " ";
	std::cout << std::endl;
	std::cout << "error_page: ";
	for (std::map<int, std::string>::iterator it = this->_errorPage.begin(); it != this->_errorPage.end(); it++)
		std::cout << it->first << " " << it->second << " ";
	std::cout << std::endl;
	std::cout << "return: ";
	for (std::map<int, std::string>::iterator it = this->_return_redirect.begin(); it != this->_return_redirect.end(); it++)
		std::cout << it->first << " " << it->second << " ";
	std::cout << std::endl;
}

std::string LocationConfig::getAlias() const { return (this->_alias); }

bool LocationConfig::getAutoindex() const { return (this->_autoindex); }

std::string LocationConfig::getUploadPath() const { return (this->_upload_path); }

std::vector<std::string> LocationConfig::getCgiExtension() const { return (this->_cgi_extension); }

std::vector<std::string> LocationConfig::getIndexes() const { return (this->_indexes); }

std::map<int, std::string> LocationConfig::getErrorPage() const { return (this->_errorPage); }

std::map<int, std::string> LocationConfig::getReturnRedirect() const { return (this->_return_redirect); }
