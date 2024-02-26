/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 17:02:41 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/26 21:03:12 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::quit_cmd(std::string msg, int fdclient){
	(void)msg;
	std::string reply;
	std::string client_nick;
	std::string client_name;
	std::string client_ip;
	
	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient){
			client_name = vec_clients[i].username;
			client_nick = vec_clients[i].nickname;
			client_ip = vec_clients[i].client_ip;
			break;
		}

	reply = ":" + client_nick + "!~" + client_name + "@" + client_ip + " QUIT :Client disconnected\r\n";

	std::cout << "\033[31m" << "disconnecing ..." << "\033[0m" << std::endl;

	for (size_t i = 0; i < vpoll.size(); i++)
		if (vpoll[i].fd == fdclient){
			close(vpoll[i].fd);
			vpoll.erase(vpoll.begin() + i);
		}

	for (size_t i = 0; i < vec_clients.size(); i++)
		if (vec_clients[i].fd == fdclient)
		{
			client_nick = vec_clients[i].nickname;
			vec_clients.erase(vec_clients.begin() + i);
		}
	
	map_clients.erase(client_nick);
	std::map<std::string, channel>::iterator it = map_channels.begin();

	while (it != map_channels.end())
	{
		if (it->second.is_client(fdclient))
		{
			it->second.remove_client(fdclient);
			it->second.remove_op(fdclient);
			it->second.remove_invited_client(fdclient);
			it->second.send_channel_msg(reply, fdclient);
		}
		it++;
	}	
}