/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 12:51:44 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/09 18:57:44 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::mode_invite(std::string channel_name, std::string mode){
	if (mode == "+i")
		map_channels[channel_name].is_invite_only = true;
	else if (mode == "-i")
		map_channels[channel_name].is_invite_only = false;
}

void server::mode_topic(std::string channel_name, std::string mode){
	if (mode == "+t")
		map_channels[channel_name].topic_restrict = true;
	else if (mode == "-t")
		map_channels[channel_name].topic_restrict = false;
}

void server::mode_password(std::string channel_name, std::string mode, std::string value, int fdclient){
	if (mode == "+k"){
		if (value.empty()){
			mode_error = true;
			error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
		}
		else
			map_channels[channel_name].set_password(value);
	}
	else if (mode == "-k")
		map_channels[channel_name].remove_password();
}

void server::mode_op(std::string channel_name, std::string mode, std::string value, int fdclient){
	if (value.empty()){
		mode_error = true;
		error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
	}
	else if (map_clients.find(value) == map_clients.end()){
		mode_error = true;
		error_reply(fdclient, "401", value, "", "No such nick");
	}
	else if (!map_channels[channel_name].is_client(map_clients[value].fd)){
		mode_error = true;	
		error_reply(fdclient, "441", value, channel_name, "They aren't on that channel");
	}
	else if (mode == "+o"){
		if (map_channels[channel_name].is_op(map_clients[value].fd) == true){	
			mode_error = true;
			error_reply(fdclient, "482", "MODE", value, "Is already an operator");
		}
		else {
			std::string reply = ":" + host() + " 273 " + channel_name + " :You are now an operator of " + "\"" + channel_name + "\"\n";
			send(map_clients[value].fd, reply.c_str(), reply.length(), 0);
			map_channels[channel_name].add_op(map_clients[value].fd);
		}
	}
	else if (mode == "-o"){
		if (map_channels[channel_name].is_op(map_clients[value].fd) == false){
			mode_error = true;
			error_reply(fdclient, "482", "MODE", value, "Is not an operator");
		}
		else if (map_clients[value].fd == fdclient){
			mode_error = true;
			error_reply(fdclient, "482", "MODE", value, "You can't remove your own operator status");
		}
		else {
			std::string reply = ":" + host() + " 273 " + channel_name + " :You are no longer an operator of " + "\"" + channel_name + "\"\n";
			send(map_clients[value].fd, reply.c_str(), reply.length(), 0);
			map_channels[channel_name].remove_op(map_clients[value].fd);
		}
	}
}

void server::mode_limit(std::string channel_name, std::string mode, std::string value, int fdclient){
	if (mode == "+l"){
		if (value.empty()){
			mode_error = true;
			error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
		}
		else {
			int limit = std::atoi(value.c_str());
			if (limit <= 0){
				mode_error = true;
				error_reply(fdclient, "461", "MODE", "", "Invalid limit");
			}
			else
				map_channels[channel_name].set_limit(limit);
		}
	}
	else if (mode == "-l")
		map_channels[channel_name].remove_limit();
}

void server::mode_cmd(std::string msg, int fdclient){
	std::stringstream split(get_value(msg));
	std::string channel_name;
	std::string mode;
	std::string client_name;
	std::string client_nick;
	std::string client_ip;
	std::string reply;

	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			client_ip = vec_clients[i].client_ip;
			break;
		}

	split >> channel_name;
	split >> mode;

	channel_name = to_lower(channel_name);
	
	if (channel_name.empty() || (channel_name == "#" && mode.empty())){
		error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
		error_reply(fdclient, "650", "MODE", "", "Syntax: <#channel> <\"+/-\"mode> [value (if needed)]");
		return;
	}
	else if (channel_name[0] != '#'){
		error_reply(fdclient, "403", "MODE", "\"" + channel_name + "\"", "Bad channel name");
		return;
	}
	else if (map_channels.find(channel_name) == map_channels.end()){
		error_reply(fdclient, "403", "MODE", "\"" + channel_name + "\"", "No such channel");
		return;
	}
	else if (!map_channels[channel_name].is_client(fdclient)){
		error_reply(fdclient, "442", client_nick, channel_name, "You're not on that channel");
		return;
	}
	else if (mode.empty()){
		std::string modes = map_channels[channel_name].get_modes();
		reply = ":" + host() + " 324 " + client_nick + " " + channel_name + " " + modes + "\n";
		send(fdclient, reply.c_str(), reply.length(), 0);
		return;
	}
	else if (!map_channels[channel_name].is_op(fdclient)){
		error_reply(fdclient, "482", "MODE", channel_name, "You're not a channel operator");
		return;
	}
	else if (mode[0] != '+' && mode[0] != '-'){
		error_reply(fdclient, "472", "MODE", "\"" + mode + "\"", "invalid input to mode command");
		return;
	}
	else {
		int mode_cmd;
		std::string da_mode;
		std::string value;
		char modes[] = {'i', 't', 'k', 'o', 'l'};
		
		for (size_t i = 1; i < mode.size(); i++)
		{
			for (mode_cmd = 0; mode_cmd < 5; mode_cmd++)
				if (mode[i] == modes[mode_cmd])
					break;
			da_mode = std::string(1, mode[0]) + std::string(1, mode[i]);
			value = "";
			switch(mode_cmd){
				case 0:
					mode_invite(channel_name, da_mode);
					break;
				case 1:
					mode_topic(channel_name, da_mode);
					break;
				case 2:
					split >> value;
					mode_password(channel_name, da_mode, value, fdclient);
					value = "";
					break;
				case 3:
					split >> value;
					mode_op(channel_name, da_mode, value, fdclient);
					break;
				case 4:
					split >> value;
					mode_limit(channel_name, da_mode, value, fdclient);
					break;
				default:
					error_reply(fdclient, "472", "MODE", "\"" + da_mode + "\"", "is unknown mode char to the server");
					continue;
			}
			if (mode_error == false){
				reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " MODE " + channel_name + " " + da_mode + " " + value + "\n";
				map_channels[channel_name].send_channel_msg(reply, fdclient);
				send(fdclient, reply.c_str(), reply.length(), 0);
			}
			else
				mode_error = false;
		}
	}
	split.clear();
}
