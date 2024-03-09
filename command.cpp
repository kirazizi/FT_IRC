/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 15:52:00 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/09 17:24:48 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

void server::handle_cmd(std::string msg, int fdclient){
	int i = 0;
	std::string cmd;
	std::string commands[] = {"JOIN", "PART", "QUIT", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "MPLAY",
	"NICK", "USER", "PASS", "PONG"};

	cmd = to_upper(get_cmd(msg));
	while(i < 13){
		if(cmd == commands[i])
			break;
		i++;
	}

	switch(i){
		case 0:
			join_cmd(msg, fdclient);
			break;
		case 1:
			part_cmd(msg, fdclient);
            break;
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
            mode_cmd(msg, fdclient);
			break;
		case 8:
			mplay_cmd(msg, fdclient);
			break;
		case 9:
			check_nick(msg, fdclient);
			break;
		case 10:
			check_user(fdclient);
			break;
		case 11:
			check_pass(fdclient);
			break;
		case 12:
			break;
		default:
            error_reply(fdclient, "421", "SERVER", get_cmd(msg), "Unknown command");
			break;
	}
}