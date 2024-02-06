/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_op.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:33:21 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/03 15:16:43 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

// void server::op_commands(int fdclient, std::string cmd, int cmd_num){

// 	size_t i = 0;
// 	for(i = 0; i < vec_clients.size(); i++)
// 		if(vec_clients[i].fd == fdclient)
// 			break;
// 	if (vec_clients[i].current_channel == ""){
// 		std::string send_msg = "You are not in a channel\n";
// 		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
// 		return;
// 	}

// 	size_t j = 0;
// 	for (j = 0; j < vec_channels.size(); j++)
// 		if (vec_channels[j].name == vec_clients[i].current_channel)
// 			break;

// 	if (!vec_channels[j].is_op(fdclient)){
// 		std::string send_msg = "You don't have the operator previlege for this channel\n";
// 		send(fdclient, send_msg.c_str(), send_msg.length(), 0);
// 		return;
// 	}
// 	else if (cmd_num == KICK){
// 		kick_client(fdclient, cmd, j);
// 		return;
// 	}
// 	else if (cmd_num == INVITE){
// 		invite_client(fdclient, cmd, j);
// 		return;
// 	}
// 	else if (cmd_num == TOPIC){
// 		topic_channel(fdclient, cmd, j);
// 		return;
// 	}
// 	else if	(cmd_num == MODE){
// 		std::string op_cmd = get_mode_cmd(cmd);
// 		op_mode(fdclient, cmd, op_cmd, j);
// 		return;
// 	}
// }