/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:51:31 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/05 11:18:05 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
# define SERVERS_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include "ServerConfig.hpp"

class Servers
{
	private:
		std::map<std::string, std::vector<ServerConfig> > _servers;
		ServerConfig _defaultServer;
		std::vector<std::string> _configStrings;

	public:
		Servers();
		~Servers();

		void ReadConfig(std::string path);
		void setServersConfig(void);

		//for debug
		void PrintConfigFile();
		void PrintServersConfig();
};

class FileOpenError : public std::exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("cannot open config file");
		}
};

class FileEmptyError : public std::exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("config file is empty");
		}
};

class ConfigContentError : public std::exception
{
	private:
		std::string _message;

	public:
		ConfigContentError(size_t pos, const std::string &line)
		{
			std::stringstream ss;
			ss << "line " << pos << ": invalid config content: " << line << std::endl;
			_message = ss.str();
		}
		virtual const char* what() const throw()
		{
			return (_message.c_str());
		}
		~ConfigContentError() throw() {}
};

#endif