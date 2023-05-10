/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 14:14:53 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/10 22:30:47 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <map>

class LocationConfig
{
	private:
		std::string _alias;
		std::string _autoindex;
		std::string _upload_path;
		std::vector<std::string> _cgi_extension;
		std::vector<std::string> _indexes;
		std::map<int, std::string> _errorPage;
		std::map<int, std::string> _return_redirect;

		void setCgiExtension(const std::string &line, const size_t &pos);
		void setIndex(const std::string &line, const size_t &pos);
		void getIntValue(const std::string &line);
		std::string getOneValue(const std::string &line, size_t &pos);
	public:
		LocationConfig(void);
		~LocationConfig(void);
		
		void setLocationConfig(const std::vector<std::string> &configStrings, size_t &pos);
		std::string getAlias() const;
		std::string getAutoindex() const;
		std::string getUploadPath() const;
		std::vector<std::string> getCgiExtension() const;
		std::vector<std::string> getIndexes() const;
		std::map<int, std::string> getErrorPage() const;
		std::map<int, std::string> getReturnRedirect() const;

		//for debug
		void PrintLocationConfig();
};

#endif