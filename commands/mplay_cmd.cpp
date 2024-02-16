/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mplay_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 21:00:12 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/16 16:41:53 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::mplay_cmd(std::string msg, int fdclient){
	std::stringstream ss(get_value(msg));
	std::string value;
	std::string response;
	int bot_fd;
	std::string arg;
	ss >> value;
	ss >> arg;
	
	std::string nick;
	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			nick = vec_clients[i].nickname;
			break;
		}
	
	if (map_clients.find("BOT") == map_clients.end()){
		msg_format(fdclient, "394", "BOT", "BOT is not connected");
		return ;
	}
	bot_fd = map_clients["BOT"].fd;
	response = nick + " " + value + " " + arg;
	ft_send(bot_fd, response.c_str());
}