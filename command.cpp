/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 15:52:00 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/06 21:41:17 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::join_channel(int fdclient, std::string cmd){

	std::string channel = get_value(cmd);
	if(channel == "")
		return;

	int i = 0;
	for(i = 0; i < (int)vec_clients.size(); i++)
		if(vec_clients[i].fd == fdclient)
			break;

	for (size_t j = 0; j < vec_channels.size(); j++)
	{
		if (vec_channels[j].name == channel)
		{
			if (vec_channels[j].is_client(fdclient))
			{
				std::string send_msg = "You are already in the channel " + channel + "\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else if (vec_channels[j].is_invite_only && !vec_channels[j].is_invited_client(fdclient))
			{
				std::string send_msg = "You are not invited to the channel " + channel + "\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
			else
			{
				vec_channels[j].add_client(fdclient);
				vec_clients[i].current_channel = channel;
				std::string send_msg = "You have joined the channel " + channel + "\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				std::cout << vec_clients[i].nickname << " has joined the channel " << channel << std::endl;
				return;
			}
		}
	}
	for (size_t j = 0; j < channel.length(); j++)
	{
		if (channel[j] == ' ')
		{
			std::string send_msg = "The channel name should be one word\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
	}
	Channel new_channel(channel);
	new_channel.add_client(fdclient);
	new_channel.add_op(fdclient);
	vec_channels.push_back(new_channel);
	std::string send_msg = "You have created the channel " + channel + "\n";
	send(fdclient, send_msg.c_str(), send_msg.length(), 0);
	std::cout << vec_clients[i].nickname << " has created the channel " << channel << std::endl;
	vec_clients[i].current_channel = channel;
}

void server::switch_channel(int fdclient, std::string cmd){
	
	std::string channel = get_value(cmd);
	if (channel == "")
		return;

	int i = 0;
	for (i = 0; i < (int)vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient)
			break;
	
	for (size_t j = 0; j < vec_channels.size(); j++)
	{
		if (vec_channels[j].name == channel)
		{
			if (vec_channels[j].is_client(fdclient))
			{
				vec_clients[i].current_channel = channel;
				std::string send_msg = "You have switched to the channel " + channel + "\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				std::cout << vec_clients[i].nickname << " has switched to the channel " << channel << std::endl;
				return;
			}
			else
			{
				std::string send_msg = "You are not in the channel " + channel + "\n";
				send(fdclient, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
		}
	}
}

void server::leave_channel(int fdclient, std::string cmd){

	std::string channel = get_value(cmd);
	if (channel == "")
		return;
		
	int i = 0;
	for (i = 0; i < (int)vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient)
			break;

	for (size_t j = 0; j < vec_channels.size(); j++)
	{
		if (vec_channels[j].is_client(fdclient))
		{
			vec_channels[j].remove_client(fdclient);
			std::string send_msg = "You have left the channel " + channel + "\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			std::cout << vec_clients[i].nickname << " has left the channel " << channel << std::endl;
			if (vec_clients[i].current_channel == channel)
				vec_clients[i].current_channel = "";
			return;
		}
		else
		{
			std::string send_msg = "You are not in the channel " + channel + "\n";
			send(fdclient, send_msg.c_str(), send_msg.length(), 0);
			return;
		}
	}
}

void server::send_message(int fdclient, std::string msg){

	if (msg == "")
		return;

	int i = 0;
	for (i = 0; i < (int)vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient)
			break;

	std::string channel = vec_clients[i].current_channel;
	if (channel == "" && vec_clients[i].is_connected == true)
	{
		std::string send_msg = "Your current channel is not set, please join a channel or switch to one \n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
		return;
	}

	for (size_t j = 0; j < vec_channels.size(); j++)
	{
		if (vec_channels[j].name == channel)
		{
			std::string send_msg = "|" + channel + "| " + vec_clients[i].nickname + ": " + msg;
			for (size_t k = 0; k < vec_channels[j].clients.size(); k++)
			{
				if (vec_channels[j].clients[k] != fdclient)
					send(vec_channels[j].clients[k], send_msg.c_str(), send_msg.length(), 0);
			}
		}
	}
}

void server::send_prv_msg(int fdclient, std::string cmd){

	std::string message = get_value(cmd);
	if(message == "")
		return;

	int i = 0;
	for(i = 0; i < (int)vec_clients.size(); i++)
		if(vec_clients[i].fd == fdclient)
			break;
	std::string sender = vec_clients[i].nickname;
	
	if (message[0] == '@')
	{
		std::string user = get_user(cmd);
		for (size_t j = 0; j < vec_clients.size(); j++)
		{
			if (vec_clients[j].nickname == user)
			{
				std::string send_msg = "|" + sender + "| " + ": " + get_message(cmd);
				send(vec_clients[j].fd, send_msg.c_str(), send_msg.length(), 0);
				return;
			}
		}
		std::string send_msg = "The user you are trying to send a message to does not exist\n";
		send(vec_clients[i].fd, send_msg.c_str(), send_msg.length(), 0);
	}
	else if (message[0] == '#')
	{
		std::string channel = get_channel(cmd);
		for (size_t j = 0; j < vec_channels.size(); j++)
		{
			if (vec_channels[j].name == channel)
			{
				if (!vec_channels[j].is_client(fdclient))
				{
					std::string send_msg = "You are not in the channel " + channel + "\n";
					send(fdclient, send_msg.c_str(), send_msg.length(), 0);
					return;
				}
				else
				{
					for (size_t k = 0; k < vec_channels[j].clients.size(); k++)
					{
						if (vec_channels[j].clients[k] != fdclient)
						{
							std::string send_msg = "|" + channel + "| " + sender + ": " + get_message(cmd);
							send(vec_channels[j].clients[k], send_msg.c_str(), send_msg.length(), 0);
						}
					}
				}
			}
		}
	}
	else
	{
		std::string send_msg = "You have to specify the channel with \"#\" or the user with \"@\"\n";
		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
	}
}

void server::handle_cmd(std::string cmd, int fdclient){
	int i = 0;
	std::string commands[] = {"JOIN", "SWITCH", "LEAVE", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	while(i < 8){
		if(get_cmd(cmd) == commands[i])
			break;
		i++;
	}
	switch(i){
		case 0:
			join_channel(fdclient, cmd);
			break;
		case 1:
			switch_channel(fdclient, cmd);
			break;
		case 2:
			leave_channel(fdclient, cmd);
			break;
		case 3:
			send_prv_msg(fdclient, cmd);
			break;
		case 4:
			op_commands(fdclient, cmd, KICK);
			break;
		case 5:
			op_commands(fdclient, cmd, INVITE);
			break;
		case 6:
			op_commands(fdclient, cmd, TOPIC);
			break;
		case 7:
			op_commands(fdclient, cmd, MODE);
			break;
		default:
			send_message(fdclient, cmd);
			break;
	}
}