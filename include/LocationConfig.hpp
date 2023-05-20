/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 14:14:53 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/20 18:43:21 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include "ServerConfig.hpp"

class ServerConfig;

class LocationConfig
{
	private:
		std::string _serverName;
		std::string _port;

		std::string _alias;
		std::string _cgi_path;
	
		std::vector<std::string> _allowedMethods;
		int _maxBodySize; //受け取るリクエストのサイズ上限。単位はメガバイトにします。不都合があれば教えてください（yuhmatsu）
		std::map<int, std::string> _errorPage;
		std::vector<std::string> _indexes;
		bool _autoindex;
		std::string _upload_path;
		std::vector<std::string> _cgi_extension;
		std::map<int, std::string> _return_redirect;

		void setCgiExtension(const std::string &line, const size_t &pos);
		void setIndex(const std::string &line, const size_t &pos);
		void getIntValue(const std::string &line);
		std::string getOneValue(const std::string &line, size_t &pos);
	public:
		LocationConfig(void);
		LocationConfig(const ServerConfig &server_config);
		~LocationConfig(void);
		
		void setLocationConfig(const std::vector<std::string> &configStrings, size_t &pos, const std::string &locationName);
		void setAllowedMethods(const std::string &line, const size_t &pos);


		//getter
		std::string getPortString() const;
		std::string getServerName() const;
		std::string getAlias() const;
		bool getAutoindex() const;
		std::string getUploadPath() const;
		std::vector<std::string> getCgiExtension() const;
		std::vector<std::string> getIndexes() const;
		std::map<int, std::string> getErrorPage() const;
		std::map<int, std::string> getReturnRedirect() const;
		std::vector<std::string> getAllowedMethods() const;
		int getMaxBodySize() const;

		//for debug
		void PrintLocationConfig();
};

#endif