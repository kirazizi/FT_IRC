/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mplayer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:23:37 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/14 21:09:59 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MPLAYER_HPP
#define MPLAYER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <signal.h>

class mplayer{
    private:
        int port;
        int sockfd;
        std::string server_ip;
        std::vector<std::string> mp3List;
    public:
        mplayer(int port, std::string server_ip) : port(port), server_ip(server_ip){}
        void run(void);
};

void signal_handler(int signum);

#endif