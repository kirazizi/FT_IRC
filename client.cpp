/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:40 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/04 00:20:39 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

std::string get_cmd(const std::string &msg){
    size_t pos = msg.find(' ');
    if (pos != std::string::npos)
        return msg.substr(0, pos);
    return msg;
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
    return channel;
}

std::string get_user(const std::string& str)
{
    std::string channel = "";
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '@')
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
    return channel;
}

int ft_strlen(char *str){
    if (!str)
        return 0;
    int i = 0;
    while (str[i])
        i++;
    return i;
}