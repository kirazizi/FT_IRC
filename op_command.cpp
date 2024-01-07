/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_command.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:33:21 by tajjid            #+#    #+#             */
/*   Updated: 2024/01/07 23:08:14 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::i_command(int fdclient, int c_in){

	if (vec_channels[c_in].is_invite_only == true){
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is no longer invite only\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].invited_clients.clear();
		vec_channels[c_in].is_invite_only = false;
	}
	else {
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is now invite only\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].is_invite_only = true;
	}
}

void server::t_command(int fdclient, int c_in){

	if (vec_channels[c_in].topic_restrict == true){
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is no longer topic restricted\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].topic = "";
		vec_channels[c_in].topic_restrict = false;
	}
	else {
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is now topic restricted\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].topic_restrict = true;
	}
}

void server::k_command(int fdclient, std::string cmd, int c_in){
	
	if (vec_channels[c_in].is_private == true){
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is not private anymore\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].password = "";
		vec_channels[c_in].is_private = false;
		return;
	}
	else if (vec_channels[c_in].is_private == false){
		std::string password = get_topic(cmd);
		if (password == ""){
			std::string send_msg = "You have to specify a password after the letter \"k\"\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else {
			std::string send_msg = "This channel " + vec_channels[c_in].name + " is now private\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			vec_channels[c_in].password = password;
			vec_channels[c_in].is_private = true;
			return;
		}
	}
}

void server::o_command(int fdclient, std::string cmd, int c_in){
	
	std::string option = get_third_word(cmd);

	if (option == "give"){
		std::string op_nick = get_user(get_topic(cmd));
		if (op_nick == ""){
			std::string send_msg = "You have to specify the user to give operator previlege with \"@\"\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else {
			size_t i = 0;
			for(i = 0; i < vec_clients.size(); i++)
				if(vec_clients[i].nickname == op_nick)
					break;
			if (i == vec_clients.size()){
				std::string send_msg = "The user " + op_nick + " doesn't exist\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else if (vec_channels[c_in].is_op(vec_clients[i].fd) == true){
				std::string send_msg = "The user " + op_nick + " is already an operator\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else if (vec_channels[c_in].is_client(vec_clients[i].fd) == false){
				std::string send_msg = "The user " + op_nick + " is not in the channel\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else {
				vec_channels[c_in].add_op(vec_clients[i].fd);
				std::string send_msg = "The user " + op_nick + " is now an operator\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				send_msg = "You are now an operator in the channel " + vec_channels[c_in].name + "\n";
				send(vec_clients[i].fd, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
		}
	}
	else if (option == "take"){
		std::string op_nick = get_user(get_topic(cmd));
		if (op_nick == ""){
			std::string send_msg = "You have to specify the user to take operator previlege with \"@\"\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else {
			size_t i = 0;
			for(i = 0; i < vec_clients.size(); i++)
				if(vec_clients[i].nickname == op_nick)
					break;
			if (i == vec_clients.size()){
				std::string send_msg = "The user " + op_nick + " doesn't exist\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else if (vec_channels[c_in].is_op(vec_clients[i].fd) == false){
				std::string send_msg = "The user " + op_nick + " is not an operator\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else if (vec_channels[c_in].is_client(vec_clients[i].fd) == false){
				std::string send_msg = "The user " + op_nick + " is not in the channel\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else {
				vec_channels[c_in].remove_op(vec_clients[i].fd);
				std::string send_msg = "The user " + op_nick + " is no longer an operator\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				send_msg = "You are no longer an operator in the channel " + vec_channels[c_in].name + "\n";
				send(vec_clients[i].fd, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
		}
	}
	else {
		std::string send_msg = "You have to specify an option \"give\" or \"take\" followed by the nickname of the user\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
}

void server::l_command(int fdclient, std::string cmd, int c_in){
	
	std::string limit = get_third_word(cmd);
	std::cout << "limit: " << limit << std::endl;
	if (vec_channels[c_in].is_limited == false || (vec_channels[c_in].is_limited == true && limit != "")){
		if (limit.find_first_not_of( "0123456789" ) != std::string::npos){
			std::string send_msg = "The limit should be a number\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		std::stringstream ss(limit);
		int limit_num;
		ss >> limit_num;
		if (limit == "" || limit_num < 0 || limit_num > 100 || ss.fail()){
			std::string send_msg = "The limit should be between 0 and 100\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else {
			std::string send_msg = "This channel " + vec_channels[c_in].name + " is now limited to " + limit + " users\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			vec_channels[c_in].limit = limit_num;
			vec_channels[c_in].is_limited = true;
			return;
		}
	}
	else if (vec_channels[c_in].is_limited == true){
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is not limited anymore\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].limit = 0;
		vec_channels[c_in].is_limited = false;
		return;
	}
}

void server::op_mode(int fdclient, std::string cmd, std::string op_cmd, int c_in){

	if (op_cmd == "i"){
		i_command(fdclient, c_in);
		return;
	}
	else if (op_cmd == "t"){
		t_command(fdclient, c_in);
		return;
	}
	else if (op_cmd == "k"){
		k_command(fdclient, cmd, c_in);
		return;
	}
	else if (op_cmd == "o"){
		o_command(fdclient, cmd, c_in);
		return;
	}
	else if (op_cmd == "l"){
		l_command(fdclient, cmd, c_in);
		return;
	}
	else
	{
		std::string send_msg = "Please specify one of these commands (i, t, k, o, l)\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
}

void server::kick_client(int fdclient, std::string cmd, size_t c_in){

	std::string message = get_value(cmd);
	
	if (message[0] == '@'){
		std::string kick_nick = get_user(message);
		
		for(size_t i = 0; i < vec_clients.size(); i++)
		{
			if(vec_clients[i].nickname == kick_nick){
				int kick_fd = vec_clients[i].fd;
				for (size_t j = 0; j < vec_channels[c_in].clients.size(); j++)
				{
					if (vec_channels[c_in].clients[j] == kick_fd){
						vec_channels[c_in].remove_client(kick_fd);
						std::string send_msg = "You have been kicked from the channel " + vec_channels[c_in].name + "\n";
						send(kick_fd, send_msg.c_str(), send_msg.length(), 0);
						if (vec_clients[j].current_channel == vec_channels[c_in].name)
							vec_clients[j].current_channel = "";
						std::cout << vec_clients[i].nickname << " has been kicked from the channel " << vec_channels[c_in].name << std::endl;
						return;
					}
				}
				std::string send_msg = "The user " + kick_nick + " is not in the channel\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
		}
		std::string send_msg = "The user " + kick_nick + " doesn't exist\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
	else if (message[0] != '@'){
		std::string send_msg = "You have to specify the user to kick with \"@\"\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
}

void server::invite_client(int fdclient, std::string cmd, size_t c_in){
		
	if (vec_channels[c_in].is_invite_only == false){
			std::string send_msg = "This channel " + vec_channels[c_in].name + " is not invite only\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
	}

	std::string message = get_value(cmd);
	
	if (message[0] == '@'){
		std::string invite_nick = get_user(message);
	
		size_t i = 0;
		for(i = 0; i < vec_clients.size(); i++)
			if(vec_clients[i].nickname == invite_nick)
				break;
		
		if (i == vec_clients.size()){
			std::string send_msg = "The user " + invite_nick + " doesn't exist\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else if (vec_channels[c_in].is_client(vec_clients[i].fd) == true){
			std::string send_msg = "The user " + invite_nick + " is already in the channel\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else if (vec_channels[c_in].is_invited_client(fdclient) == true){
			std::string send_msg = "The user " + invite_nick + " is already invited to the channel\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		vec_channels[c_in].add_invited_client(vec_clients[i].fd);
		std::string send_msg = "The user " + invite_nick + " has been invited to the channel\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		send_msg = "You have been invited to the channel " + vec_channels[c_in].name + "\n";
		send(vec_clients[i].fd, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
	else if (message[0] != '@'){
		std::string send_msg = "You have to specify the user to invite with \"@\"\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
}

void server::topic_channel(int fdclient, std::string cmd, size_t c_in){
	
	vec_channels[c_in].topic_restrict = true;
	if (vec_channels[c_in].topic_restrict == false){
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is not topic restricted\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
	
	std::string opt = get_second_word(cmd);
	
	if (opt == "view"){
		if (vec_channels[c_in].topic == ""){
			std::string send_msg = "The topic of the channel " + vec_channels[c_in].name + " is not set yet\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		std::string send_msg = "The topic of the channel " + vec_channels[c_in].name + " is " + vec_channels[c_in].topic;
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
	else if (opt == "set"){
		std::string topic = get_topic(cmd);
	
		if (topic == ""){
			std::string send_msg = "You have to specify a topic\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
		else {
			std::string send_msg;
			if (vec_channels[c_in].topic != "")
				send_msg = "The topic of the channel " + vec_channels[c_in].name + " is now " + topic;
			else
				send_msg = "The topic of the channel " + vec_channels[c_in].name + " has been set to " + topic;
			vec_channels[c_in].topic = topic;
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
	}
	else {
		std::string send_msg = "You have to specify a topic \"view\" or \"set\"\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
}

void server::op_commands(int fdclient, std::string cmd, int cmd_num){

	size_t i = 0;
	for(i = 0; i < vec_clients.size(); i++)
		if(vec_clients[i].fd == fdclient)
			break;
	if (vec_clients[i].current_channel == ""){
		std::string send_msg = "You are not in a channel\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}

	size_t j = 0;
	for (j = 0; j < vec_channels.size(); j++)
		if (vec_channels[j].name == vec_clients[i].current_channel)
			break;

	if (!vec_channels[j].is_op(fdclient)){
		std::string send_msg = "You don't have the operator previlege for this channel\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}
	else if (cmd_num == KICK){
		kick_client(fdclient, cmd, j);
		return;
	}
	else if (cmd_num == INVITE){
		invite_client(fdclient, cmd, j);
		return;
	}
	else if (cmd_num == TOPIC){
		topic_channel(fdclient, cmd, j);
		return;
	}
	else if	(cmd_num == MODE){
		std::string op_cmd = get_mode_cmd(cmd);
		op_mode(fdclient, cmd, op_cmd, j);
		return;
	}
}