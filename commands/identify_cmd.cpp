/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_cmd.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:21:22 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/09 17:24:40 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::check_user(int fdclient){
	// ERR_ALREADYREGISTRED
	msg_format(fdclient, "462", "+_+", "You may not reregister");
}

void server::check_pass(int fdclient){
	// ERR_ALREADYREGISTRED
	msg_format(fdclient, "462", "+_+", "You may not reregister");
}

void server::check_nick(std::string msg, int fdclient){
	// change nick
	std::stringstream ss(msg);
	std::string cmd;
	std::string nick;
	std::string param;
	ss >> cmd;
	ss >> nick;
	ss >> param;
	
	// nick name policy
	if (nick_policy(nick, fdclient) == 1){
		ss.clear();
		return ;
	}
	
	// check if nickname is already taken
	for (size_t i = 0; i < vec_clients.size(); i++){
        if (vec_clients[i].nickname == nick){
            msg_format(fdclient, "433", "+_+" , "Nickname already taken please try again!");
            ss.clear();
            return ;
        }
    }

	// check if nickname contains space
	if (param != ""){
        msg_format(fdclient, "432", "+_+" , "Nickname cannot contain space please try again!");
        ss.clear();
        return ;
    }
	
	// get old nickname
	std::string old_nick;
	for (size_t i = 0; i < vec_clients.size(); i++){
		if (vec_clients[i].fd == fdclient){
			old_nick = vec_clients[i].nickname;
			break;
		}
	}
	
	// change nickname
	for (size_t i = 0; i < vec_clients.size(); i++){
		if (vec_clients[i].fd == fdclient){
			vec_clients[i].nickname = nick;
			map_clients[nick] = vec_clients[i];
			msg_format(fdclient, "001", "+_+" , "Nickname changed successfully!");
			for (size_t j = 0; j < vec_clients.size(); j++){
				std::string msg = ":" + old_nick + "!~" + vec_clients[i].username + "@" + vec_clients[i].client_ip + " NICK " + ":" + nick;
                ft_send(vec_clients[j].fd, msg + "\r\n");
			}
		}
	}
	ss.clear();
}
