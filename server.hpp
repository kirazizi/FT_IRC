/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:05:49 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/01 15:45:44 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "headers.hpp"
#include "client.hpp"

class server {
    public:
        std::vector<int> clientfds;
        std::vector<pollfd> vpoll;
        std::vector<client> vec_clients;

        int port;
        std::string srv_pass;
        server(void) : port(0), srv_pass("") {};
        server(int port, std::string password) : port(port), srv_pass(password) {};
        int server_setup();
        void server_polling(int fdsocket);
        void server_accept(int fdsocket);
        int  server_recieve(int fdclient);
        void identify_client(std::string msg,int fdclient);
};

#endif