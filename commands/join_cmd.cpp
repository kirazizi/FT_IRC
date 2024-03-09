/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:48:51 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/09 19:19:10 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::join_channel_msg(std::string channel_name, std::string client_name, std::string client_nick, int fdclient){
	std::string reply;
	std::string host_post = host();

	reply += ":" + client_nick + "!~" + client_name + "@" + map_clients[client_nick].client_ip + " JOIN " + channel_name + "\r\n";
	reply += ":" + host_post + " 332 " + client_nick + " " + channel_name + " " + map_channels[channel_name].topic + "\n";
	reply += ":" + host_post + " 353 " + client_nick + " = " + channel_name + " :" + map_channels[channel_name].get_clients_names() + "\n";
	reply += ":" + host_post + " 366 " + client_nick + " = " + channel_name + " :" + "End of /NAMES list." + "\n";
	send(fdclient, reply.c_str(), reply.size(), 0);

	reply = ":" + client_nick + "!~" + client_name + "@" + map_clients[client_nick].client_ip + " JOIN " + channel_name + "\n";
	map_channels[channel_name].send_channel_msg(reply, fdclient);
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
		if (channel_name[0] != '#' || channel_name == "#"){ 																						// checking if the channel name is not valid
			error_reply(fdclient, "403", "JOIN", "\"" + channel_name + "\"", "Bad channel name");
			continue;
		}
		else if (map_channels.find(channel_name) == map_channels.end()){ 																			//checking if the channel is new
			map_channels[channel_name] = channel(channel_name);
			map_channels[channel_name].topic = ":No topic is set";
			map_channels[channel_name].limit = 0;
			map_channels[channel_name].add_op(fdclient);
		}
		else if (map_channels[channel_name].is_client(fdclient)){ 																					// checking if the client is already in the channel
			error_reply(fdclient, "443", "JOIN " , "\"" + channel_name + "\"", "You are already in that channel");
			continue;
		}
		else if (map_channels[channel_name].is_limited && map_channels[channel_name].clients.size() >= (size_t)map_channels[channel_name].limit){ 	// checking if the channel is full
			error_reply(fdclient, "471", "JOIN", "\"" + channel_name + "\"", "This channel is full");
			continue;
		}
		else if (map_channels[channel_name].is_invite_only && !map_channels[channel_name].is_invited_client(fdclient)){ 							// checking if the client is invited
			error_reply(fdclient, "473", "JOIN", "\"" + channel_name + "\"", "You are not invited to this channel");
			error_reply(fdclient, "473", "JOIN", client_nick , "Try to ask one these ops: " + map_channels[channel_name].get_ops());
			continue;
		}
		else if (map_channels[channel_name].is_private){																							// checking if the channel is private
			if (keys.size() == 0 || i >= keys.size() || !map_channels[channel_name].is_password(keys[i])){
				error_reply(fdclient, "475", "JOIN", "\"" + channel_name + "\"", "Wrong key");
				continue;
			}
		}

		map_channels[channel_name].add_client(fdclient, client_nick);
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
	else
		while (std::getline(value, value_str, ',')) 																								// putting the channels in a vector
			if (value_str != ""){
				value_str = to_lower(value_str);
				channels.push_back(value_str);
			}

	value.clear();
	split >> value_str;
	value << value_str;

	while (std::getline(value, value_str, ',')) 																									// putting the keys in a vector
		keys.push_back(value_str);

	join_the_channels(channels, keys, fdclient);
	split.clear();
	value.clear();
}
