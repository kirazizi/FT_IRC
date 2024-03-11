/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:32:17 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/11 14:02:40 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../client/client.hpp"

void server::leave_the_channels(std::vector<std::string> channels, int fdclient, std::string reason){
	std::string reply;
	std::string channel_name;
	std::string client_name;
	std::string client_nick;
	std::string client_ip;
	
	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			client_ip = vec_clients[i].client_ip;
			break;
		}

	for (size_t i = 0; i < channels.size(); i++)
	{
		channel_name = channels[i];
		if (channel_name.empty() || channel_name[0] != '#') {														// checking if the channel name is valid
			error_reply(fdclient, "403", "PART", + "\"" + channel_name + "\"" , "Bad channel name");
			continue;
		}
		else if (map_channels.find(channel_name) == map_channels.end()){											// checking if the channel exists
			error_reply(fdclient, "403", "PART", + "\"" + channel_name + "\"" , "No such channel");
			continue;
		}
		else if (!map_channels[channel_name].is_client(fdclient)){													// checking if the client is in the channel
			error_reply(fdclient, "442", "PART", channel_name, "You are not in that channel");
			continue;
		}
		reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " PART " + channel_name + " :" + reason + "\n";
		map_channels[channel_name].send_channel_msg(reply, fdclient);
		send(fdclient, reply.c_str(), reply.length(), 0);
		map_channels[channel_name].remove_client(fdclient);
		map_channels[channel_name].remove_op(fdclient);
		map_channels[channel_name].remove_invited_client(fdclient);
		if (map_channels[channel_name].op_clients.size() == 0 && map_channels[channel_name].clients.size() > 0){
			map_channels[channel_name].add_op(map_channels[channel_name].clients[0].first);
			reply = ":" + host() + " 381 " + channel_name + " :You are now an operator of " + "\"" + channel_name + "\"\n";
			send(map_channels[channel_name].clients[0].first, reply.c_str(), reply.length(), 0);
			reply = ":\"TheServer\"!~\"TheServer\"@" + host() + " MODE " + channel_name + " " + "+o " + map_channels[channel_name].clients[0].second + "\n";
			map_channels[channel_name].send_channel_msg(reply, fdclient);
		}
		else if (map_channels[channel_name].clients.size() == 0)													// checking if the last client leaves the channel
			map_channels.erase(channel_name);
	}
}
	

void server::part_cmd(std::string msg, int fdclient){

	std::stringstream split(get_value(msg));
	std::vector<std::string> channels;
	std::string reason;
	std::stringstream value;
	std::string value_str;

	split >> value_str;
	value << value_str;
	split >> reason;

	if (value_str.empty() || (value_str == "#" && reason.empty()) || (value_str == ":" && reason.empty())){			// checking if the command has enough parameters
		error_reply(fdclient, "461", "PART", "", "Not enough parameters");
		return;
	}
	else { 																											// adding the channels to the vector
		while (std::getline(value, value_str, ','))
			channels.push_back(value_str);
	}

	if (reason.empty() || reason == "#" || reason == ":")															// checking if the reason is empty
		reason = "No reason";
	else {
		reason = msg.substr(msg.find(value_str) + value_str.length() + 1);
		if (reason[0] == ':')
			reason = reason.substr(1);
		reason.erase(std::remove(reason.begin(), reason.end(), '\r'), reason.end());
		reason.erase(std::remove(reason.begin(), reason.end(), '\n'), reason.end());
	}

	leave_the_channels(channels, fdclient, reason);
	split.clear();
	value.clear();
}
