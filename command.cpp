/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 15:52:00 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/12 16:19:16 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::handle_cmd(std::string msg, int fdclient){
	int i = 0;
	std::string commands[] = {"JOIN", "QUIT", "PRIVMSG", "KICK", "INVITE", "TOPIC", "PART", "MODE"};
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
			quit_cmd(msg, fdclient);
			break;
		case 2:
			privmsg_cmd(msg, fdclient);
			break;
		case 3:
			kick_cmd(msg, fdclient);
			break;
		case 4:
			invite_cmd(msg, fdclient);
			break;
		case 5:
			topic_cmd(msg, fdclient);
			break;
		case 6:
			break;
        case 7:
            break;
		default:
			break;
	}
}