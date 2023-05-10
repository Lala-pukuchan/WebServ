/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyanagim <hyanagim@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:51:31 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/10 13:12:05 by hyanagim         ###   ########.fr       */
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
		ServerConfig _defaultServer;
		vector<string> _configStrings;

	public:
		Servers();
		~Servers();

		void ReadConfig(string path);
		void setServersConfig(void);

		map<string, vector<ServerConfig> >& getServers();

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

#endif