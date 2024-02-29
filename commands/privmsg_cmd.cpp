/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:09:16 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/29 22:53:57 by tajjid           ###   ########.fr       */
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
		if (users[i][0] == '#') {
			channel_name = to_lower(users[i]);
			if (channel_name == "#"){
				error_reply(fdclient, "403", "PRIVMSG", "\"" + channel_name + "\"", "Bad channel name");
			}
			else if (map_channels.find(channel_name) == map_channels.end()){
				error_reply(fdclient, "401", client_nick , "\"" + channel_name + "\"", "No such channel");
			}
			else if (!map_channels[channel_name].is_client(fdclient)){
				error_reply(fdclient, "442", client_nick , channel_name, "You are not in that channel");
			}
			else {
				reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " PRIVMSG " + channel_name + " :" + message;
				map_channels[channel_name].send_channel_msg(reply, fdclient);
			}
		}
		else {
			joined_name = users[i];
			if (map_clients.find(joined_name) == map_clients.end()){
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

    split >> value_str;
    value << value_str;
    split >> message;

    if (value_str == "" || (value_str == "#" && message.empty())){
        error_reply(fdclient, "411", "PRIVMSG", "", "No recipient given");
        return;
    }
    else {
        while (std::getline(value, value_str, ','))
            users.push_back(value_str);    
    }

    if (msg.find(msg) == std::string::npos || message == ""){
        error_reply(fdclient, "412", "PRIVMSG", "", "No text to send");
        return;
    }
	else if (message[0] == ':')
		message = msg.substr(msg.find(value_str) + value_str.length() + 2);
    else 
        message = msg.substr(msg.find(value_str) + value_str.length() + 1);

    send_privmsgs(users, message, fdclient);
    split.clear();
    value.clear();
}
