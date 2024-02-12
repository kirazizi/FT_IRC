/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:53:29 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/12 21:24:53 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::kick_users(std::string channel_name, std::vector<std::string> users, std::string reason, int fdclient){
	std::string reply;
	std::string client_name;
	std::string client_nick;
	std::string kicked_user;
	int fd_kicked_user;

	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			break;
		}

	if (map_channels.find(channel_name) == map_channels.end()){
		error_reply(fdclient, "403", "KICK " + channel_name, "", "No such channel");
		return;
	}
	
	for (size_t i = 0; i < users.size(); i++)
	{
		kicked_user = users[i];
		fd_kicked_user = map_clients[kicked_user].fd;
		if (map_channels[channel_name].is_client(fd_kicked_user) == false){
			error_reply(fdclient, "441", client_nick, kicked_user, "They aren't on that channel");
			continue;
		}
		else {
			reply = ":" + client_nick + " KICK " + channel_name + " " + kicked_user + " " + reason + "\n";
			for (size_t j = 0; j < map_channels[channel_name].clients.size(); j++)
				send(map_channels[channel_name].clients[j].first, reply.c_str(), reply.length(), 0);
			map_channels[channel_name].remove_client(fd_kicked_user);
			map_channels[channel_name].remove_op(fd_kicked_user);
		}
	}
}

void server::kick_cmd(std::string msg, int fdclient){
	std::stringstream split(get_value(msg));
	std::vector<std::string> users;
	std::string channel_name;
	std::string reason;
	std::string param;

	if (get_value(msg) == "" || get_value(msg).find(" ") == std::string::npos){
		error_reply(fdclient, "461", "KICK", "" ,"Not enough parameters");
		return;
	}
	else {
		std::getline(split, channel_name, ' ');
		while (std::getline(split, param, ','))
		{
			if (param.find(' ') != std::string::npos){
				users.push_back(param.substr(0, param.find(' ')));
				reason = param.substr(param.find(' ') + 1);
				while (std::getline(split, param, ','))
					reason += " " + param;
				break;
			}
			users.push_back(param);
		}
	}
	if (channel_name[0] != '#'){
		error_reply(fdclient, "403", "KICK " + channel_name, "", "Bad channel name");
		return;
	}
	else 
		if (map_channels[channel_name].is_op(fdclient) == false){
			error_reply(fdclient, "482", "KICK " + channel_name, "", "You're not a channel operator");
			return;
		}
	kick_users(channel_name, users, reason, fdclient);
}
