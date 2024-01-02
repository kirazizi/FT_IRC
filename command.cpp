/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 15:52:00 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/02 00:43:48 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::join_channel(int fdclient, std::string cmd){
    for (size_t i = 0; i < vec_clients.size(); i++)
    {
        if (vec_clients[i].fd == fdclient)
        {
            std::string value = get_value(cmd);
            if (value == "")
                return;
            
            for (size_t j = 0; j < vec_clients[i].channels.size(); j++)
            {
                if (vec_clients[i].channels[j] == value)
                {
                    std::string send_msg = "You have already joined the channel " + value + "\n";
                    send(fdclient, send_msg.c_str(), send_msg.length(), 0);
                    return;
                }
            }
            
            vec_clients[i].channels.push_back(value);
            vec_clients[i].channel = value;
            std::string send_msg = "You have joined the channel " + value + "\n";
            send(fdclient, send_msg.c_str(), send_msg.length(), 0);

            for (size_t j = 0; j < vec_clients.size(); j++)
            {
                for (size_t k = 0; k < vec_clients[j].channels.size(); k++)
                {
                    if (vec_clients[j].channels[k] == value && vec_clients[j].fd != fdclient)
                    {
                        std::string send_msg = vec_clients[i].nickname + " has joined the channel " + value + "\n";
                        send(vec_clients[j].fd, send_msg.c_str(), send_msg.length(), 0);
                    }
                }
            }
            std::cout << vec_clients[i].nickname << " has joined the channel " << value << std::endl;
        }
    }
}

void server::send_message(int fdclient, std::string msg){
    int i = 0;

    for (i = 0; i < (int)vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient)
            break;

    std::string channel = vec_clients[i].channel;

    for (size_t j = 0; j < vec_clients.size(); j++)
    {
        for (size_t k = 0; k < vec_clients[j].channels.size(); k++)
        {
            if (vec_clients[j].channels[k] == channel && vec_clients[j].fd != fdclient)
            {
                std::string send_msg = "{" + channel + "}: " + vec_clients[i].nickname + ": " + msg;
                send(vec_clients[j].fd, send_msg.c_str(), send_msg.length(), 0);
                break;
            }
        }
    }
      
}

void server::switch_channel(int fdclient, std::string cmd){
    int i = 0;
    
    for (i = 0; i < (int)vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient)
            break;

    std::string channel = get_value(cmd);
    if (channel == "")
        return;

    for (size_t j = 0; j < vec_clients[i].channels.size(); j++)
    {
        if (vec_clients[i].channels[j] == channel)
        {
            vec_clients[i].channel = channel;
            std::string send_msg = "You have switched to channel " + channel;
            send(fdclient, send_msg.c_str(), send_msg.length(), 0);
            return;
        }
        else
            send(fdclient, "The channel you are trying to switch to does not exist\n", 57, 0);
    }
}

void server::leave_channel(int fdclient, std::string channel){
    int i = 0;
    
    for (i = 0; i < (int)vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient)
            break;
    
    for (size_t j = 0; j < vec_clients[i].channels.size(); j++)
    {
        if (vec_clients[i].channels[j] == channel)
        {
            vec_clients[i].channels.erase(vec_clients[i].channels.begin() + j);
            std::string send_msg = "You have left the channel " + channel + "\n";
            send(fdclient, send_msg.c_str(), send_msg.length(), 0);
            return;
        }
        else
            send(fdclient, "The channel you are trying to leave does not exist\n", 55, 0);
    }
}

void send_msg(){
    std::cout << "MSG" << std::endl;
}

void quit(){
    std::cout << "QUIT" << std::endl;
}

void server::handle_cmd(std::string cmd, int fdclient){
    int i = 0;
    std::string commands[] = {"JOIN", "LEAVE", "MSG", "QUIT", "SWITCH"};
    while(i < 5){
        if(get_cmd(cmd) == commands[i])
            break;
        i++;
    }
    switch(i){
        case 0:
            join_channel(fdclient, cmd);
            break;
        case 1:
            leave_channel(fdclient, cmd);
            break;
        case 2:
            // send_msg();
            break;
        case 3:
            // quit();
            break;
        case 4:
            switch_channel(fdclient, cmd);
            break;
        default:
            send_message(fdclient, cmd);
    }
}