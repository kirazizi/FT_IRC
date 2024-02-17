/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:48:51 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/17 13:50:45 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

std::string server::get_clients_names(std::vector<std::pair<int, std::string> > clients){
	std::string reply;

	for (size_t i = 0; i < clients.size(); i++)
	{
		reply += clients[i].second;
		if (i != clients.size() - 1)
			reply += " ";
	}
	return reply;
}

void server::join_channel_msg(std::string channel_name, std::string client_name, std::string client_nick, int fdclient){
	std::string reply;
	std::string host_post = host();
	int client_fd;

	reply += ":" + client_nick + "!~" + client_name + "@" + map_clients[client_nick].client_ip + " JOIN " + channel_name + "\r\n";
	reply += ":" + host_post + " 332 " + client_nick + " " + channel_name + " " + map_channels[channel_name].topic.second + "\n";
	reply += ":" + host_post + " 353 " + client_nick + " = " + channel_name + " :" + get_clients_names(map_channels[channel_name].clients) + "\n";
	reply += ":" + host_post + " 366 " + client_nick + " = " + channel_name + " :" + "End of /NAMES list." + "\n";
	send(fdclient, reply.c_str(), reply.size(), 0);

	reply = ":" + client_nick + "!~" + client_name + "@" + map_clients[client_nick].client_ip + " JOIN " + channel_name + "\n";
	for (size_t j = 0; j < map_channels[channel_name].clients.size(); j++)
	{
		client_fd = map_channels[channel_name].clients[j].first;
		if (client_fd != fdclient){
			send(client_fd, reply.c_str(), reply.size(), 0);
		}
	}
}

void server::join_the_channels(std::vector<std::string> channels, std::vector<std::string> keys, int fdclient){
	std::string reply;
	std::string channel_name;
	std::string client_name;
	std::string client_nick;
	
	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			break;
		}
	
	for (size_t i = 0; i < channels.size(); i++)
	{
		channel_name = channels[i];
		if (channel_name[0] != '#'){
			error_reply(fdclient, "403", "JOIN", "\"" + channel_name + "\"", "Bad channel name");
			continue;
		}
		else if (map_channels.find(channel_name) == map_channels.end()){
			map_channels[channel_name] = channel(channel_name);
			map_channels[channel_name].topic.first = "";
			map_channels[channel_name].topic.second = ":No topic is set";
			map_channels[channel_name].limit = 0;
			map_channels[channel_name].add_op(fdclient);
		}
		else if (map_channels[channel_name].is_client(fdclient)){
			error_reply(fdclient, "443", "JOIN " , "\"" + channel_name + "\"", "You are already in that channel");
			continue;
		}
		else if (map_channels[channel_name].is_limited && map_channels[channel_name].clients.size() >= (size_t)map_channels[channel_name].limit){
			error_reply(fdclient, "471", "JOIN", "\"" + channel_name + "\"", "This channel is full");
			continue;
		}
		else if (map_channels[channel_name].is_invite_only && !map_channels[channel_name].is_invited_client(fdclient)){
			error_reply(fdclient, "473", "JOIN", "\"" + channel_name + "\"", "You are not invited to this channel");
			error_reply(fdclient, "473", "JOIN", client_nick , "Try to ask one these ops: " + map_channels[channel_name].get_ops());
			continue;
		}
		else if (map_channels[channel_name].is_private){
			if (keys.size() == 0 || i >= keys.size() || !map_channels[channel_name].is_password(keys[i])){
				error_reply(fdclient, "475", "JOIN", "\"" + channel_name + "\"", "Wrong key");
				continue;
			}
		}

		map_channels[channel_name].add_client(fdclient, client_nick);
		if (map_channels[channel_name].op_clients.size() == 0)
			map_channels[channel_name].add_op(map_channels[channel_name].clients.begin()->first);
		join_channel_msg(channel_name, client_name, client_nick, fdclient);
	}
}

void server::join_cmd(std::string msg, int fdclient){

	std::stringstream split(get_value(msg));
	std::vector<std::string> channels;
	std::vector<std::string> keys;
    std::stringstream value;
    std::string value_str;

    split >> value_str;
    value << value_str;
    if (value_str == "" || value_str == "#")
    {
        error_reply(fdclient, "461", "JOIN", "", "Not enough parameters");
        return;
    }
    else {
        while (std::getline(value, value_str, ','))
            channels.push_back(value_str);
    }
    
    value.clear();
    split >> value_str;
    value << value_str;

    while (std::getline(value, value_str, ','))
        keys.push_back(value_str);

    join_the_channels(channels, keys, fdclient);
    split.clear();
    value.clear();
}
