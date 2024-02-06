/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:05:49 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/06 19:57:37 by tajjid           ###   ########.fr       */
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

class channel;

class server {
	public:
		std::vector<int> clientfds;
		std::vector<pollfd> vpoll;
		std::vector<client> vec_clients;
		std::map<std::string, client> map_clients;
		std::map<std::string, channel> map_channels;

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
		void join_cmd(std::string msg, int fdclient);
		void privmsg_cmd(std::string msg, int fdclient);
	

		/*			JOIN FUNCTIONS			*/
		void join_the_channels(std::vector<std::string> channels, std::vector<std::string> keys, int fdclient, std::string msg);
		void join_channel_msg(std::string channel_name, std::string client_name, std::string client_nick, int fdclient);
		std::string get_clients_names(std::vector<std::pair<int, std::string> > clients);

		/*			PRIVMSG FUNCTIONS		*/
		void send_privmsgs(std::vector<std::string> users, std::string message, int fdclient, std::string msg);
};

#endif