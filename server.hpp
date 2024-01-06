/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:05:49 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/06 23:45:17 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "headers.hpp"
#include "client.hpp"
#include "channel.hpp"

#define KICK 1
#define INVITE 2
#define TOPIC 3
#define MODE 4

class Channel;

class server {
	public:
		std::vector<int> clientfds;
		std::vector<pollfd> vpoll;
		std::vector<client> vec_clients;
		std::vector<Channel> vec_channels;

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
		void switch_channel(int fdclient, std::string cmd);
		void leave_channel(int fdclient, std::string cmd);
		void send_prv_msg(int fdclient, std::string cmd);
		void kick_client(int fdclient, std::string cmd, size_t c_in);
		void invite_client(int fdclient, std::string cmd, size_t c_in);
		void topic_channel(int fdclient, std::string cmd, size_t c_in);
		void op_commands(int fdclient, std::string cmd, int cmd_num);
		void i_command(int fdclient, int c_in);
		void t_command(int fdclient, int c_in);
		void k_command(int fdclient, std::string cmd, int c_in);
		void op_mode(int fdclient, std::string cmd, std::string op_cmd, int c_in);
		void send_message(int fdclient, std::string msg);
};

#endif