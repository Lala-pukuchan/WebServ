/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyanagim <hyanagim@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 11:32:59 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/10 13:55:08 by hyanagim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"
#include "Webserv.hpp"

int main(int argc, char **argv)
{
	Servers servers;

	if (argc != 2)
	{
		std::cout << "usage: ./webserv [config_file]" << std::endl;
		return (1);
	}
	servers.ReadConfig(argv[1]);
	try
	{
		servers.setServersConfig();
		servers.PrintServersConfig();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	Webserv webserv(servers);
	webserv.makeServerSocket();
	return (0);
}