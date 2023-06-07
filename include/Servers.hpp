/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:51:31 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/06/07 19:44:36 by yuhmatsu         ###   ########.fr       */
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
using namespace std;

class Servers
{
	private:
		map<string, vector<ServerConfig> > _servers;
		bool _defaultServerFlag;
		ServerConfig _defaultServer;
		vector<string> _configStrings;

	public:
		Servers();
		~Servers();

		void ReadConfig(string path);
		void setServersConfig(void);

		map<string, vector<ServerConfig> >& getServers();
		ServerConfig getDefaultServer();
		bool checkPortServer();

		//for debug
		void PrintConfigFile();
		void PrintServersConfig();
};

class FileOpenError : public exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("cannot open config file");
		}
};

class FileEmptyError : public exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("config file is empty");
		}
};

class EmptyServerError : public exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("Server is not defined");
		}
};

class SameServerError : public exception
{
	public:
		virtual const char* what() const throw()
		{
			return ("The same server exists.");
		}
};

class ConfigContentError : public exception
{
	private:
		string _message;

	public:
		ConfigContentError(size_t pos, const string &line)
		{
			stringstream ss;
			ss << "line " << pos << ": invalid config content: " << line << endl;
			_message = ss.str();
		}
		virtual const char* what() const throw()
		{
			return (_message.c_str());
		}
		~ConfigContentError() throw() {}
};

class SemicolonError : public std::exception
{
	private:
		std::string _message;

	public:
		SemicolonError(size_t pos, const std::string &line)
		{
			std::stringstream ss;
			ss << "line " << pos << ": error with ';' :" << line << std::endl;
			_message = ss.str();
		}
		virtual const char* what() const throw()
		{
			return (_message.c_str());
		}
		~SemicolonError() throw() {}
};

class EmptyValueError : public std::exception
{
	private:
		std::string _message;

	public:
		EmptyValueError(size_t pos, const std::string &line)
		{
			std::stringstream ss;
			ss << "line " << pos << ": value is empty :" << line << std::endl;
			_message = ss.str();
		}
		virtual const char* what() const throw()
		{
			return (_message.c_str());
		}
		~EmptyValueError() throw() {}
};

class EmptyPortError : public std::exception
{
	private:
		std::string _message;

	public:
		virtual const char* what() const throw()
		{
			return ("port is empty");
		}
		~EmptyPortError() throw() {}
};

class InvalidKeyError : public std::exception
{
	private:
		std::string _message;

	public:
		InvalidKeyError(size_t pos, const std::string &line)
		{
			std::stringstream ss;
			ss << "line " << pos << ": invalid key :" << line << std::endl;
			_message = ss.str();
		}
		virtual const char* what() const throw()
		{
			return (_message.c_str());
		}
		~InvalidKeyError() throw() {}
};

class InvalidMethodError : public std::exception
{
	private:
		std::string _message;

	public:
		InvalidMethodError(size_t pos, const std::string &line)
		{
			std::stringstream ss;
			ss << "line " << pos << ": invalid method :" << line << std::endl;
			_message = ss.str();
		}
		virtual const char* what() const throw()
		{
			return (_message.c_str());
		}
		~InvalidMethodError() throw() {}
};

class EmptyAliasError : public std::exception
{
	private:
		std::string _message;

	public:
		virtual const char* what() const throw()
		{
			return ("alias is empty");
		}
		~EmptyAliasError() throw() {}
};

#endif