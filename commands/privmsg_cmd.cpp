/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:09:16 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/10 20:21:01 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::send_privmsgs(std::vector<std::string> users, std::string message, int fdclient){
	std::string channel_name;
	std::string joined_name;
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

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i][0] == '#') {																			// checking if the recipient is a channel
			channel_name = to_lower(users[i]);
			if (channel_name == "#"){
				error_reply(fdclient, "403", "PRIVMSG", "\"" + channel_name + "\"", "Bad channel name");
			}
			else if (map_channels.find(channel_name) == map_channels.end()){								// checking if the channel exists
				error_reply(fdclient, "403", client_nick , "\"" + channel_name + "\"", "No such channel");
			}
			else if (!map_channels[channel_name].is_client(fdclient)){										// checking if the client is in the channel
				error_reply(fdclient, "442", client_nick , channel_name, "You are not in that channel");
			}
			else {
				reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " PRIVMSG " + channel_name + " :" + message;
				map_channels[channel_name].send_channel_msg(reply, fdclient);
			}
		}
		else {																								// The recipient is a user
			joined_name = users[i];
			if (map_clients.find(joined_name) == map_clients.end()){										// checking if the client is in the server
				error_reply(fdclient, "401", client_nick, joined_name, "No such nick");
			}
			else {
				reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " PRIVMSG " + joined_name + " :" + message;
				send(map_clients[joined_name].fd, reply.c_str(), reply.length(), 0);
			}
		}
	}
}

void server::privmsg_cmd(std::string msg, int fdclient){

	std::stringstream split(get_value(msg));
	std::vector<std::string> users;
	std::string message;
	std::stringstream value;
	std::string value_str;
	std::string founded;

	split >> value_str;
	founded = value_str;
	value << value_str;
	split >> message;

	if (value_str == "" || (value_str == "#" && message.empty())){											// checking if the command has enough parameters
		error_reply(fdclient, "411", "PRIVMSG", "", "No recipient given");
		return;
	}
	else {																									// adding the recipients to the vector
		while (std::getline(value, value_str, ','))
			users.push_back(value_str);    
	}

	if (message.empty()){																					// checking if there is a message to send
		error_reply(fdclient, "412", "PRIVMSG", "", "No text to send");
		return;
	}
	else if (message[0] == ':')
		message = msg.substr(msg.find(founded) + founded.length() + 1);
	else 
		message = msg.substr(msg.find(founded) + founded.length());

	send_privmsgs(users, message, fdclient);
	split.clear();
	value.clear();
}
