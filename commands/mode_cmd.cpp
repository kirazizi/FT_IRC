/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 12:51:44 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/13 19:52:10 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

// void mode_invite(std::string channel_name, std::string mode, std::string value, int fdclient){
// }

// void mode_topic(std::string channel_name, std::string mode, std::string value, int fdclient){
// }

// void mode_password(std::string channel_name, std::string mode, std::string value, int fdclient){
// }

// void mode_op(std::string channel_name, std::string mode, std::string value, int fdclient){
// }

// void mode_limit(std::string channel_name, std::string mode, std::string value, int fdclient){
// }

void server::mode_cmd(std::string msg, int fdclient){
	std::stringstream split(get_value(msg));
	std::string channel_name;
	std::string mode;
	std::string value;
	std::string client_name;
	std::string client_nick;
	std::string reply;

	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			break;
		}

	split >> channel_name;
	split >> mode;
	split >> value;
	
	if (channel_name.empty() || channel_name == "#"){
		error_reply(fdclient, "461", "MODE", "", "Not enough parameters");
		error_reply(fdclient, "650", "MODE", "", "<target> [[(+|-)]<modes> [<mode-parameters>]]");
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
	else {
		int mode_type = 0;
		std::string modes[] = {"i", "t", "k", "o", "l"};
		for (mode_type; mode_type < 5; mode_type++)
			if (mode.size() == 2 && mode[1] == modes[mode_type][0])
				break;

			switch(mode_type){
				case 0:
					// mode_invite(channel_name, mode, value, fdclient);
					break;
				case 1:
					// mode_topic(channel_name, mode, value, fdclient);
					break;
				case 2:
					// mode_password(channel_name, mode, value, fdclient);
					break;
				case 3:
					// mode_op(channel_name, mode, value, fdclient);
					break;
				case 4:
					// mode_limit(channel_name, mode, value, fdclient);
					break;
				default:
					error_reply(fdclient, "472", "MODE", channel_name, "is unknown mode char to the server");
					break;
			}
	}
}
