/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:17 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/07 20:20:26 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "headers.hpp"
#include <sstream>
#include <unistd.h>

class client {
	public:
		int fd;
		int is_connected;
		std::string buffer_cl;
		std::string nickname;
		std::string username;
		std::string password;
		std::string current_channel;
		client(int fd) : fd(fd), is_connected(0), nickname(""), username(""), password("") {};
};

std::string get_cmd(const std::string& str);
std::string get_mode_cmd(const std::string& str);
std::string get_value(const std::string& str);
std::string get_message(const std::string& str);
std::string get_channel(const std::string& str);
std::string get_user(const std::string& str);
std::string get_first_word(const std::string& str);
std::string get_second_word(const std::string& str);
std::string get_third_word(const std::string& str);
std::string get_topic(const std::string& str);
int ft_strlen(char *str);

#endif