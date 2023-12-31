/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:17 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/31 15:38:37 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "headers.hpp"
#include <sstream>

class client {
    public:
        int fd;
        std::string nickname;
        std::string username;
        std::string password;
        client(int fd) : fd(fd), nickname(""), username(""), password("") {};
};

std::string get_cmd(const std::string& str);
std::string get_value(const std::string& str);
int ft_strlen(char *str);

#endif