/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:40 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/07 18:47:26 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "headers.hpp"

std::string get_cmd(const std::string &msg){
	size_t pos = msg.find(' ');
	if (pos != std::string::npos)
		return msg.substr(0, pos);
	return msg.substr(0, msg.length());
}

std::string get_mode_cmd(const std::string &msg){
	size_t pos = msg.find(' ');
	for (size_t i = pos + 1; i < msg.length(); i++)
	{
		if (msg[i] == ' ')
			return msg.substr(pos + 1, i - pos - 1);
	}
	return msg.substr(0, pos);
}

std::string get_value(const std::string &msg){
	size_t pos = msg.find(' ');
	if (pos == std::string::npos)
		return "";
	std::string value = msg.substr(pos + 1);
	value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
	value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
	return value;
}

int ft_strlen(char *str){
	if (!str)
		return 0;
	int i = 0;
	while (str[i])
		i++;
	return i;
}

void error_reply(int fdclient, std::string code_error, std::string nick_name, std::string value, std::string msg){
	std::string reply;
	reply = ":" + std::string("myirc.1337.ma") + " " + code_error + " " + nick_name + " " + value + " :" + msg + "\n";
	send(fdclient, reply.c_str(), reply.length(), 0);
}
