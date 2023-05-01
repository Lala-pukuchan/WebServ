/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 10:46:22 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/01 18:28:08 by yuhmatsu         ###   ########.fr       */
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

class ServerConfig
{
	private:
		std::string _serverName;
		std::string _port;
		std::vector<std::string> _allowedMethod;
		int _maxBodySize; //受け取るリクエストのサイズ上限
		std::map<int, std::string> _errorPages;  //なんで，mapで持っているのか
		std::map<std::string, LocationConfig> _locations; // cgiとかここにまとまるのか？　まだ，ここまで踏み込んでいない
		std::vector<std::string> _indexes; //最初に表示するページだけど，複数ある
		std::string _autoindex; //なんか課題で言われているやつ

		std::map<int, std::string> return_redirects_;
		std::string upload_path_;
		bool cgi; //要る？
		std::vector<std::string> cgi_extension_;

		bool isAllSemicolon(std::string &line);
	public:
		ServerConfig();
		~ServerConfig();
		void setServerConfig(std::vector<std::string> &configStrings, size_t &pos);
		std::string getPortString();
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
};

#endif