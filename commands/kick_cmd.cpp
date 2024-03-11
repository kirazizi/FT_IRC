/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:53:29 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/11 14:02:29 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../client/client.hpp"

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
	
	for (size_t i = 0; i < users.size(); i++)
	{
		kicked_user = users[i];
		if (map_clients.find(kicked_user) == map_clients.end()){									// checking if the client is not in the server
			error_reply(fdclient, "401", client_nick, kicked_user, "No such nick");
			continue;
		}
		fd_kicked_user = map_clients[kicked_user].fd;
		if (fd_kicked_user == fdclient){															// checking if the client is kicking himself
			error_reply(fdclient, "442", client_nick, kicked_user, "You can't kick yourself");
			continue;
		}
		else if (map_channels[channel_name].is_client(fd_kicked_user) == false){					// checking if the client is not in the channel
			error_reply(fdclient, "441", client_nick, kicked_user, "They aren't on that channel");
			continue;
		}
		else {
			map_channels[channel_name].remove_client(fd_kicked_user);
			map_channels[channel_name].remove_op(fd_kicked_user);
			reply = ":" + client_nick + " KICK " + channel_name + " " + kicked_user + " :" + reason + "\n";
			map_channels[channel_name].send_channel_msg(reply, fdclient);
			send(fdclient, reply.c_str(), reply.length(), 0);
			send(fd_kicked_user, reply.c_str(), reply.length(), 0);
		}
	}
}

void server::kick_cmd(std::string msg, int fdclient){
	std::stringstream split(get_value(msg));
	std::vector<std::string> users;
	std::string channel_name;
	std::string reason;
	std::stringstream value;
	std::string value_str;

	split >> channel_name;
	channel_name = to_lower(channel_name);
	split >> value_str;
	value << value_str;
	split >> reason;

	if (channel_name.empty() || value_str.empty() || (channel_name == "#" && value_str.empty())){ 	// checking if the command has enough parameters
		error_reply(fdclient, "461", "KICK", "" ,"Not enough parameters");
		return;
	}
	else {
		while (std::getline(value, value_str, ','))
			users.push_back(value_str);
	}

	if (channel_name[0] != '#'){																	// checking if the channel name is valid
		error_reply(fdclient, "403", "KICK", "\"" + channel_name + "\"", "Bad channel name");
		return;
	}
	else if (map_channels.find(channel_name) == map_channels.end()){								// checking if the channel exists
		error_reply(fdclient, "403", "KICK", "\"" + channel_name + "\"", "No such channel");
		return;
	}
	else if (map_channels[channel_name].is_op(fdclient) == false){									// checking if the client is an operator
			error_reply(fdclient, "482", "KICK", channel_name, "You're not a channel operator");
			return;
	}

    if (reason.empty() || reason == ":")
        reason = "no reason";
    else {
        reason = msg.substr(msg.find(value_str) + value_str.length() + 1);
        if (reason[0] == ':')
            reason = reason.substr(1);
        reason.erase(std::remove(reason.begin(), reason.end(), '\r'), reason.end());
		reason.erase(std::remove(reason.begin(), reason.end(), '\n'), reason.end());
    }

	kick_users(channel_name, users, reason, fdclient);
	split.clear();
	value.clear();
}
