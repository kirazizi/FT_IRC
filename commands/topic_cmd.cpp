/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:42:35 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/09 19:19:24 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::topic_cmd(std::string msg, int fdclient){
	std::stringstream split(get_value(msg));
	std::string reply;
	std::string client_nick;
	std::string client_name;
	std::string channel_name;
	std::string client_ip;
	std::string topic;

	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			client_ip = vec_clients[i].client_ip;
			break;
		}

	split >> channel_name;
	channel_name = to_lower(channel_name);

	if (channel_name.empty() || channel_name == ":" || channel_name == "#"){			// checking if the command has enough parameters
		error_reply(fdclient, "461", "TOPIC", "", "Not enough parameters");
		return;
	}
	else if (channel_name[0] != '#'){													// checking if the channel name is valid
		error_reply(fdclient, "403", "TOPIC", channel_name, "Bad channel name");
		return;
	}
	else if (map_channels.find(channel_name) == map_channels.end()){					// checking if the channel exists
		error_reply(fdclient, "403", "TOPIC", channel_name, "No such channel");
		return;
	}

	if (!(split >> topic)){
		if (map_channels[channel_name].is_client(fdclient)){
			reply = ":" + host() + " 332 " + client_nick + " " + channel_name + " " + map_channels[channel_name].topic + "\n";
			send(fdclient, reply.c_str(), reply.size(), 0);
		}
		else
			error_reply(fdclient, "442", client_nick, "", "You're not on that channel");
	}
	else
	{
		if (map_channels[channel_name].topic_restrict && !map_channels[channel_name].is_op(fdclient)){
			error_reply(fdclient, "482", "TOPIC", "", "You're not a channel operator");
			return;
		}
		topic = get_value(msg).substr(channel_name.size());
		topic = topic.substr(topic.find_first_not_of(" "));
		map_channels[channel_name].topic = topic;
	
		reply = ":" + client_nick + " TOPIC " + channel_name + " " + topic + "\r\n";
		send(fdclient, reply.c_str(), reply.size(), 0);
		reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " TOPIC " + channel_name + " " + topic + "\r\n";
		map_channels[channel_name].send_channel_msg(reply, fdclient);
	}
	split.clear();
}