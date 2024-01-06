/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_command.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:33:21 by tajjid            #+#    #+#             */
/*   Updated: 2024/01/06 23:29:22 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::i_command(int fdclient, int c_in){

	if (vec_channels[c_in].is_invite_only == true){
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is no longer invite only\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
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
		vec_channels[c_in].topic_restrict = false;
	}
	else {
		std::string send_msg = "This channel " + vec_channels[c_in].name + " is now topic restricted\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		vec_channels[c_in].topic_restrict = true;
	}
}

// void server::k_command(){
// }

// void server::o_command(){
// }

// void server::l_command(){
// }

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
		std::cout << "k command" << std::endl;
	}
	else if (op_cmd == "o"){
		std::cout << "o command" << std::endl;
	}
	else if (op_cmd == "l"){
		std::cout << "l command" << std::endl;
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
						vec_channels[c_in].clients.erase(vec_channels[c_in].clients.begin() + j);
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