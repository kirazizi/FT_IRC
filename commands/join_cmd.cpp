/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:48:51 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/12 21:23:34 by tajjid           ###   ########.fr       */
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
	reply += ":" + client_nick + "!~" + client_name + "@127.0.0.1" + " JOIN " + channel_name + "\r\n";
	reply += ":" + host_post + " 332 " + client_nick + " " + channel_name + " " + map_channels[channel_name].topic.second + "\n";
	reply += ":" + host_post + " 353 " + client_nick + " = " + channel_name + " :" + get_clients_names(map_channels[channel_name].clients) + "\n";
	reply += ":" + host_post + " 366 " + client_nick + " = " + channel_name + " :" + "End of /NAMES list." + "\n";
	send(fdclient, reply.c_str(), reply.size(), 0);
}

void server::join_the_channels(std::vector<std::string> channels, std::vector<std::string> keys, int fdclient){
	std::string reply;
	std::string channel_name;
	std::string client_name;
	std::string client_nick;
	(void)keys;
	
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
			error_reply(fdclient, "403", "JOIN " + client_nick, channel_name, "Bad channel name");
			continue;
		}
		else if (map_channels.find(channel_name) == map_channels.end()){
			map_channels[channel_name] = channel(channel_name);
			map_channels[channel_name].topic.first = "";
			map_channels[channel_name].topic.second = ":No topic is set";
			map_channels[channel_name].add_op(fdclient);
		}
		else if (map_channels[channel_name].is_client(fdclient)){
			error_reply(fdclient, "443", "JOIN " , channel_name, "You are already in that channel");
			continue;
		}
		map_channels[channel_name].add_client(fdclient, client_nick);
		if (map_channels[channel_name].op_clients.size() == 0)
			map_channels[channel_name].add_op(map_channels[channel_name].clients.begin()->first);
		join_channel_msg(channel_name, client_name, client_nick, fdclient);
	
		int client_fd;
		reply = ":" + client_nick + "!~" + client_name + "@127.0.0.1" + " JOIN " + channel_name + "\n";
		for (size_t j = 0; j < map_channels[channel_name].clients.size(); j++)
		{
			client_fd = map_channels[channel_name].clients[j].first;
			if (client_fd != fdclient){
				send(client_fd, reply.c_str(), reply.size(), 0);
			}
		}
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
    std::cout << "value_str1: " << value_str << std::endl;
    if (value_str == "" || value_str == "#")
    {
        error_reply(fdclient, "461", "JOIN", "", "Not enough parameters");
        return;
    }

	std::string param;
    while (std::getline(value, param, ','))
        channels.push_back(param);
    
    split >> value_str;
    std::cout << "value_str2: " << value_str << std::endl;
    value.clear();
    value << value_str;

    while (std::getline(value, param, ','))
        keys.push_back(param);

    join_the_channels(channels, keys, fdclient);
	// for (size_t i = 0; i < channels.size(); i++)
	// 	std::cout << "channels: " << channels[i] << std::endl;
	// for (size_t i = 0; i < keys.size(); i++)
	// 	std::cout << "keys: " << keys[i] << std::endl;
	// if (get_value(msg) == "" || get_value(msg) == "#")
	// {
	// 	error_reply(fdclient, "461", "JOIN", "", "Not enough parameters");
	// 	return;
	// }
	// else if (get_value(msg).size() > 50)
	// {
	// 	error_reply(fdclient, "405", "JOIN", get_value(msg), "Channel name is too long");
	// 	return;
	// }
	// while (std::getline(split, param, ','))
	// {
	// 	if (param.find(' ') != std::string::npos)
	// 	{
	// 		std::string temp = param.substr(0, param.find(' '));
	// 		channels.push_back(temp);
	// 		keys.push_back(param.substr(param.find(' ') + 1));
	// 		while (std::getline(split, param, ','))
	// 		{
	// 			if (param.find(' ') != std::string::npos)
	// 			{
	// 				temp = param.substr(0, param.find(' '));
	// 				keys.push_back(temp);
	// 				break;
	// 			}
	// 			keys.push_back(param);
	// 		}
	// 		break;
	// 	}
	// 	channels.push_back(param);
	// }
	// join_the_channels(channels, keys, fdclient);
}
