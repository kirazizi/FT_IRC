/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 17:02:41 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/09 19:21:40 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../client.hpp"

void server::quit_cmd(std::string msg, int fdclient){
    (void)msg;
    std::string client_nick;

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
        }
        it++;
    }
}