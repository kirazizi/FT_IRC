/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 15:09:37 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/11 13:51:06 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

std::string get_ip(struct in_addr add){
    char *ip = inet_ntoa(add);
    return std::string(ip);
}

std::string host(){
    // gethostname() function
    char host[1024];
    if (gethostname(host, 1024) < 0){
        std::cout << "Error: getting hostname" << std::endl;
    }
    return std::string(host);
}

void ft_send(int fdclient, std::string msg){
    if (send(fdclient, msg.c_str(), msg.length(), 0) < 0){
        std::cout << "Error: sending message" << std::endl;
    }
}

void msg_format(int fdclient, std::string cmd, std::string nick, std::string msg){
    std::string prefix = host();
    std::string response = ":" + prefix + " " + cmd + " " + nick + " :" + msg + "\r\n";
    ft_send(fdclient, response);
}

int nick_policy(std::string nick, int fdclient){
    if (!isalpha(nick[0])){
        msg_format(fdclient, "432", "+_+" , "Nickname must start with a letter please try again!");
        return 1;
    }
    if (nick.length() > 9){
        msg_format(fdclient, "432", "+_+" , "Nickname is too long please try again!");
        return 1;
    }
    return 0;
}

void server::clear_all_client(){
    // clear all clients
    for (size_t i = 0; i < vpoll.size(); i++)
        close(vpoll[i].fd);
    vpoll.clear();
    vec_clients.clear();
    map_clients.clear();
    map_channels.clear();
    exit(1);
}
