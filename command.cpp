/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 15:52:00 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/12 19:34:44 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::handle_cmd(std::string msg, int fdclient){
	int i = 0;
	std::string commands[] = {"JOIN", "PART", "QUIT", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	while(i < 8){
		if(get_cmd(msg) == commands[i])
			break;
		i++;
	}
	switch(i){
		case 0:
			join_cmd(msg, fdclient);
			break;
		case 1:
			// part_cmd(msg, fdclient);
		case 2:
			quit_cmd(msg, fdclient);
			break;
		case 3:
			privmsg_cmd(msg, fdclient);
			break;
		case 4:
			kick_cmd(msg, fdclient);
			break;
		case 5:
			invite_cmd(msg, fdclient);
			break;
		case 6:
			topic_cmd(msg, fdclient);
			break;
		case 7:
			break;
		default:
			break;
	}
}