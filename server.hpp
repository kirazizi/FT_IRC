/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:05:49 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/30 21:23:19 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "headers.hpp"
#include "client.hpp"

class server : public client{
    public:
        std::vector<int> clientfds;
        std::vector<pollfd> vpoll;
        int port;
        std::string srv_pass;
        server(void);
        server(int port, std::string password);
        int server_setup();
        void server_polling(int fdsocket);
        void server_accept(int fdsocket);
        void server_recieve(int fdclient);
        // std::string username;
        // std::string nickname;
        // std::string password;
};

#endif