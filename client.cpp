/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:40 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/07 23:05:26 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

std::string get_first_word(const std::string &msg){
	size_t pos = msg.find(' ');
	if (pos != std::string::npos)
		return msg.substr(0, pos);
	return msg.substr(0, msg.length() - 1);
}

std::string get_second_word(const std::string &msg){
	size_t pos = msg.find(' ');
	if (pos == 0 || pos == std::string::npos)
		return "";
	for (size_t i = pos + 1; i < msg.length(); i++)
	{
		if (msg[i] == ' ' )
			return msg.substr(pos + 1, i - pos - 1);
	}
	return msg.substr(pos + 1, msg.length() - pos - 2);
}

std::string get_third_word(const std::string& str)
{
	size_t pos = str.find(' ');
	pos = str.find(' ', pos + 1);
	if (pos == 0 || pos == std::string::npos)
		return "";
	for (size_t i = pos + 1; i < str.length(); i++)
	{
		if (str[i] == ' ')
			return str.substr(pos + 1, i - pos - 1);
	}
	return str.substr(pos + 1, str.length() - pos - 2);
}

std::string get_topic(const std::string &msg){
	size_t pos = msg.find(' ');
	pos = msg.find(' ', pos + 1);
	if (pos != std::string::npos && pos != 0)
		return msg.substr(pos + 1);
	return "";
}

std::string get_cmd(const std::string &msg){
	size_t pos = msg.find(' ');
	if (pos != std::string::npos)
		return msg.substr(0, pos);
	return msg.substr(0, msg.length() - 1);
}

std::string get_mode_cmd(const std::string &msg){
	size_t pos = msg.find(' ');
	for (size_t i = pos + 1; i < msg.length(); i++)
	{
		if (msg[i] == ' ')
			return msg.substr(pos + 1, i - pos - 1);
	}
	return msg.substr(pos + 1, msg.length() - pos - 2);
}

std::string get_value(const std::string &msg){
	size_t pos = msg.find(' ');
	if(pos == std::string::npos)
		return "";
	std::string value = msg.substr(pos + 1);
	value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
	value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
	return value;
}

std::string get_message(const std::string& str)
{
	size_t pos = str.find(' ');
	pos = str.find(' ', pos + 1);
	if (pos != std::string::npos)
		return str.substr(pos + 1);
	return "";
}

std::string get_channel(const std::string& str)
{
	std::string channel = "";
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '#')
		{
			i++;
			for (size_t j = i; j < str.length(); j++)
			{
				if (str[j] == ' ')
					return channel;
				channel += str[j];
			}
		}
	}
	channel.erase(std::remove(channel.begin(), channel.end(), '\n'), channel.end());
	return channel;
}

std::string get_user(const std::string& str)
{
	std::string user = "";
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '@')
		{
			i++;
			for (size_t j = i; j < str.length(); j++)
			{
				if (str[j] == ' ')
					return user;
				user += str[j];
			}
		}
	}
	user.erase(std::remove(user.begin(), user.end(), '\n'), user.end());
	return user;
}

int ft_strlen(char *str){
	if (!str)
		return 0;
	int i = 0;
	while (str[i])
		i++;
	return i;
}