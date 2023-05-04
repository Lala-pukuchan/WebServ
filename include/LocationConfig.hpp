/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 14:14:53 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/04 22:50:38 by yuhmatsu         ###   ########.fr       */
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
		std::map<int, std::string> _error_page;
		std::map<int, std::string> _return_redirect;

		void setCgiExtension(const std::string &line, const size_t &pos);
		void getIntValue(const std::string &line);
		std::string getOneValue(const std::string &line, size_t &pos);
	public:
		LocationConfig(void);
		~LocationConfig(void);
		
		void setLocationConfig(const std::vector<std::string> &configStrings, size_t &pos);
};

#endif