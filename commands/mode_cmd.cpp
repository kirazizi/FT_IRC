/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 12:51:44 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/13 22:23:18 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

// void server::mode_invite(std::string channel_name, std::string mode, std::string value, int fdclient){
// 	if (mode == "+i")
// 		map_channels[channel_name].is_invite_only = true;
// 	else if (mode == "-i")
// 		map_channels[channel_name].is_invite_only = false;
// 	else 
// 		error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
// }

// void server::mode_topic(std::string channel_name, std::string mode, std::string value, int fdclient){
// 	if (mode == "+t")
// 		map_channels[channel_name].topic_restrict = true;
// 	else if (mode == "-t")
// 		map_channels[channel_name].topic_restrict = false;
// 	else 
// 		error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
// }

// void server::mode_password(std::string channel_name, std::string mode, std::string value, int fdclient){
// 	if (mode == "+k"){
// 		if (value.empty())
// 			error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
// 		else
// 			map_channels[channel_name].set_password(value);
// 	}
// 	else if (mode == "-k")
// 		map_channels[channel_name].remove_password();
// 	else 
// 		error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
// }

// void server::mode_op(std::string channel_name, std::string mode, std::string value, int fdclient){
	
// 	if (value.empty())
// 		error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
// 	else if (map_clients.find(value) == map_clients.end())
// 		error_reply(fdclient, "401", value, "", "No such nick");
// 	else if (mode == "+o")
// 		map_channels[channel_name].add_op(map_clients[value].fd);
// 	else if (mode == "-o")
// 		map_channels[channel_name].remove_op(map_clients[value].fd);
// 	else 
// 		error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
// }


// void server::mode_limit(std::string channel_name, std::string mode, std::string value, int fdclient){
// 	if (mode == "+l"){
// 		if (value.empty())
// 			error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
// 		else {
// 			int limit = std::stoi(value);	
// 			if (limit < 0)
// 				error_reply(fdclient, "461", "MODE", "", "Invalid limit");
// 			else
// 				map_channels[channel_name].set_limit(limit);
// 		}
// 	}
// 	else if (mode == "-l")
// 		map_channels[channel_name].remove_limit();
// 	else 
// 		error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
// }

// void server::mode_cmd(std::string msg, int fdclient){
// 	std::stringstream split(get_value(msg));
// 	std::string channel_name;
// 	std::string mode;
// 	std::string value;
// 	std::string client_name;
// 	std::string client_nick;
// 	std::string reply;

// 	for (size_t i = 0; i < vec_clients.size(); i++)
// 		if (vec_clients[i].fd == fdclient){
// 			client_name = vec_clients[i].username;
// 			client_nick = vec_clients[i].nickname;
// 			break;
// 		}

// 	split >> channel_name;
// 	split >> mode;
// 	split >> value;
	
// 	if (channel_name.empty() || channel_name == "#"){
// 		error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
// 		error_reply(fdclient, "650", "MODE", "", "<target> [[(+|-)]<modes> [<mode-parameters>]]");
// 		return;
// 	}
// 	else if (!map_channels[channel_name].is_client(fdclient)){
// 		error_reply(fdclient, "442", client_nick, channel_name, "You're not on that channel");
// 		return;
// 	}
// 	else if (mode.empty()){
// 		std::string modes = map_channels[channel_name].get_modes();
// 		reply = ":" + host() + " 324 " + client_nick + " " + channel_name + " " + modes + "\n";
// 		send(fdclient, reply.c_str(), reply.length(), 0);
// 		return;
// 	}
// 	else if (!map_channels[channel_name].is_op(fdclient)){
// 		error_reply(fdclient, "482", "MODE", channel_name, "You're not a channel operator");
// 		return;
// 	}
// 	else {
// 		int mode_type = 0;
// 		std::string modes[] = {"i", "t", "k", "o", "l"};
// 		for (mode_type; mode_type < 5; mode_type++)
// 			if (mode.size() == 2 && mode[1] == modes[mode_type][0])
// 				break;

// 			switch(mode_type){
// 				case 0:
// 					mode_invite(channel_name, mode, value, fdclient);
// 					break;
// 				case 1:
// 					mode_topic(channel_name, mode, value, fdclient);
// 					break;
// 				case 2:
// 					mode_password(channel_name, mode, value, fdclient);
// 					break;
// 				case 3:
// 					mode_op(channel_name, mode, value, fdclient);
// 					break;
// 				case 4:
// 					mode_limit(channel_name, mode, value, fdclient);
// 					break;
// 				default:
// 					error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
// 					break;
// 			}
// 	}
// }
