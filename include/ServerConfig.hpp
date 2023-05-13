/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:46:22 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/13 18:49:06 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include "LocationConfig.hpp"

class LocationConfig;

class ServerConfig
{
	private:
		std::string _serverName;
		std::string _port;
		std::map<std::string, LocationConfig> _locations; // cgiとかここにまとまるのか？　まだ，ここまで踏み込んでいない
		std::string _alias;

		std::vector<std::string> _allowedMethods;
		int _maxBodySize; //受け取るリクエストのサイズ上限。単位はメガバイトにします。不都合があれば教えてください（yuhmatsu）
		std::map<int, std::string> _errorPage;  //なんで，mapで持っているのか
		std::vector<std::string> _indexes; //最初に表示するページだけど，複数ある
		bool _autoindex; //なんか課題で言われているやつ
		std::string _upload_path;
		std::vector<std::string> _cgi_extension;
		std::map<int, std::string> _return_redirect;

		void setLocationConfig(const std::string &value, const std::vector<std::string> &configStrings, size_t &pos);
		void setCgiExtension(const std::string &line, const size_t &pos);
		void setIndex(const std::string &line, const size_t &pos);
		void setAllowedMethods(const std::string &line, const size_t &pos);
		void getIntValue(const std::string &line);
		std::string getOneValue(const std::string &line, size_t &pos);

	public:
		ServerConfig();
		ServerConfig& operator=(const ServerConfig &src);
		~ServerConfig();
		

		void setServerConfig(const std::vector<std::string> &configStrings, size_t &pos);

		//getter
		std::string getPortString() const;
		std::string getServerName() const;
		std::vector<std::string> getAllowedMethods() const;
		int getMaxBodySize() const;
		std::vector<std::string> getIndexes() const;
		std::map<std::string, LocationConfig> getLocationConfigs() const;
		std::map<int, std::string> getErrorPage() const;
		std::map<int, std::string> getReturnRedirect() const;
		std::string getUploadPath() const;
		bool getAutoindex() const;
		std::vector<std::string> getCgiExtension() const;

		//for debug
		void PrintServerConfig();
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