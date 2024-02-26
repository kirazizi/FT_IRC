/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:17 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/26 15:55:53 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "headers.hpp"
#include <sstream>
#include <unistd.h>

class client {
	public:
		std::string buffer_cl;
		int fd;
		int is_connected;
		std::string client_msg;
		std::string nickname;
		std::string username;
		std::string password;
		std::string current_channel;
		std::string bot;
		std::string client_ip;
		client(){};
		client(int fd) : fd(fd), is_connected(0), nickname(""), username(""), password("") {};
};

std::string get_cmd(const std::string& str);
std::string get_value(const std::string& str);
int ft_strlen(char *str);
void error_reply(int fdclient, std::string code_error, std::string nick_name, std::string value, std::string msg);

#endif