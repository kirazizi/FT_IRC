/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:17 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/02 12:14:25 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "headers.hpp"
#include <sstream>

class client {
    public:
        int fd;
        int is_connected;
        std::string buffer_cl;
        std::string nickname;
        std::string username;
        std::string password;
        std::string channel;
        std::vector<std::string> channels;
        client(int fd) : fd(fd), is_connected(0), nickname(""), username(""), password("") {};
};

std::string get_cmd(const std::string& str);
std::string get_value(const std::string& str);
std::string get_message(const std::string& str);
std::string get_channel(const std::string& str);
int ft_strlen(char *str);

#endif