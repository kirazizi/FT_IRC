/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:05:49 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/02 00:35:50 by tajjid           ###   ########.fr       */
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
        void handle_cmd(std::string msg, int fdclient);
        void join_channel(int fdclient, std::string cmd);
        void leave_channel(int fdclient, std::string cmd);
        // void send_msg(int fdclient, std::string cmd);
        // void quit(int fdclient, std::string cmd);
        void switch_channel(int fdclient, std::string cmd);
        void send_message(int fdclient, std::string msg);
};

#endif