/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:40 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/11 13:44:22 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "../src/headers.hpp"
#include "../server/server.hpp"

std::string get_cmd(const std::string &msg){
	size_t pos = msg.find(' ');
	if (pos != std::string::npos){
		return msg.substr(0, pos);
	}
	std::string cmd = msg.substr(0, pos);
	cmd.erase(std::remove(cmd.begin(), cmd.end(), '\r'), cmd.end());
	cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());
	return cmd;
}

std::string to_upper(std::string str){
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::toupper(str[i]);
	return str;
}

std::string to_lower(std::string str){
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::tolower(str[i]);
	return str;
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
	std::string host_post = host();
	reply = ":" + host_post + " " + code_error + " " + nick_name + " " + value + " :" + msg + "\n";
	send(fdclient, reply.c_str(), reply.length(), 0);
}
