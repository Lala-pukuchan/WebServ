/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuhmatsu <yuhmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 11:32:59 by yuhmatsu          #+#    #+#             */
/*   Updated: 2023/05/01 16:28:22 by yuhmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Servers.hpp"

int main(int argc, char **argv)
{
	Servers servers;

	if (argc != 2)
	{
		std::cout << "usage: ./webserv [config_file]" << std::endl;
		return (1);
	}
	servers.ReadConfig(argv[1]);
	servers.PrintConfig();
	return (0);
}