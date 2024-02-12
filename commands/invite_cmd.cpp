/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:19:35 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/12 12:39:35 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

//:irc.leet.com 341 taha nana #haha

void server::invite_cmd(std::string msg, int fdclient){
    std::stringstream split(get_value(msg));
    std::string reply;
    std::string client_nick;
    std::string channel_name;
    std::string invited_name;

    for (size_t i = 0; i < vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient){
            client_nick = vec_clients[i].nickname;
            break;
        }

    split >> invited_name;
    split >> channel_name;

    int fdinvited;
    for (size_t i = 0; i < vec_clients.size(); i++)
        if (vec_clients[i].nickname == invited_name){
            fdinvited = vec_clients[i].fd;
            break;
        }

    if (channel_name.empty() || invited_name.empty() || invited_name[0] == ':' || channel_name[0] == ':'){
        error_reply(fdclient, "461", "INVITE", "", "Not enough parameters");
        return;
    }
    if (channel_name[0] != '#' || channel_name == "#" || map_channels.find(channel_name) == map_channels.end()){
        error_reply(fdclient, "403", "INVITE", client_nick + " " + channel_name, "No such channel");
        return;
    }
    else if (!map_channels[channel_name].is_client(fdclient)){
        error_reply(fdclient, "442", client_nick, "", "You are not in that channel");
        return;
    }
    else if (!map_channels[channel_name].is_op(fdclient)){
        error_reply(fdclient, "482", "INVITE", "", "You're not the channel operator");
        return;
    }
    else if (map_clients.find(invited_name) == map_clients.end()){
        error_reply(fdclient, "401", "INVITE", "(" + invited_name + ")", "No such nick");
        return;
    }
    else if (map_channels[channel_name].is_client(fdinvited)){
        error_reply(fdclient, "443", "INVITE", invited_name, "is already on the channel");
        return;
    }
    else if (map_channels[channel_name].is_invited_client(map_clients[invited_name].fd)){
        error_reply(fdclient, "443", "INVITE", invited_name, "is already invited");
        return;
    }
    else {
       map_channels[channel_name].add_invited_client(map_clients[invited_name].fd);
        reply = ":" + host() + " " + std::string("341") + " " + client_nick + " " + invited_name + " " + channel_name + "\n";
        send(fdclient, reply.c_str(), reply.size(), 0);
        reply = ":" + client_nick + " " + "INVITE" + " " + invited_name + " " + channel_name + "\n";
        send(fdinvited, reply.c_str(), reply.size(), 0);
    }
}