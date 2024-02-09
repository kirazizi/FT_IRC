/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:09:16 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/08 13:21:46 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::send_privmsgs(std::vector<std::string> users, std::string message, int fdclient){
	std::string channel_name;
	std::string joined_name;
	std::string client_name;
	std::string client_nick;
	std::string reply;

	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			break;
		}

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i][0] == '#') {
			channel_name = users[i];
			if (map_channels.find(channel_name) == map_channels.end()){
				error_reply(fdclient, "401", client_nick , channel_name, "No such channel");
			}
			else if (!map_channels[channel_name].is_client(fdclient)){
				error_reply(fdclient, "442", client_nick , channel_name, "You are not in that channel");
			}
			else {
				reply = ":" + client_nick + "!~" + client_name + "@127.0.0.1" + " PRIVMSG " + channel_name + " :" + message + "\n";
				for (size_t j = 0; j < map_channels[channel_name].clients.size(); j++)
				{
					if (map_channels[channel_name].clients[j].first != fdclient)
						send(map_channels[channel_name].clients[j].first, reply.c_str(), reply.length(), 0);
				}
			}
		}
		else {
			joined_name = users[i];
			if (map_clients.find(joined_name) == map_clients.end()){
				error_reply(fdclient, "401", client_nick, joined_name, "No such nick");
			}
			else {
				reply = ":" + client_nick + "!~" + client_name + "@127.0.0.1" + " PRIVMSG " + joined_name + " :" + message + "\n";
				send(map_clients[joined_name].fd, reply.c_str(), reply.length(), 0);
			}
		}
	}
}

void server::privmsg_cmd(std::string msg, int fdclient){
	std::vector<std::string> users;
	std::string param;
	std::string message = get_value(msg);
	std::string users_name = message.substr(0,message.find(' '));

	std::stringstream split(users_name);
	
	if (message.find(' ') != std::string::npos)
		message = message.substr(message.find(' ') + 1);
	else
		message = "";

	while (std::getline(split, param, ','))
		users.push_back(param);

	if (users.size() == 0)
		error_reply(fdclient, "411", "JOIN", "", "No recipient given");
	else if (message == "")
		error_reply(fdclient, "412", "JOIN", "", "No text to send");
	else
		send_privmsgs(users, message, fdclient);

}
