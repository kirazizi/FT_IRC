/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:40 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/30 14:14:00 by sbzizal          ###   ########.fr       */
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

int ft_strlen(char *str){
    if (!str)
        return 0;
    int i = 0;
    while (str[i])
        i++;
    return i;
}