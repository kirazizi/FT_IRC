/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:27:47 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/26 17:04:47 by sbzizal          ###   ########.fr       */
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
		std::string buffer;
		bool mode_error;
		// std::string reply;

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
		
		
		/*			HELPER FUNCTIONS		*/
		
		void clear_all_client();
	

		/*			JOIN FUNCTIONS			*/
		void join_cmd(std::string msg, int fdclient);
		void join_the_channels(std::vector<std::string> channels, std::vector<std::string> keys, int fdclient);
		void join_channel_msg(std::string channel_name, std::string client_name, std::string client_nick, int fdclient);
		std::string get_clients_names(std::vector<std::pair<int, std::string> > clients);

		/*			PRIVMSG FUNCTIONS		*/
		void privmsg_cmd(std::string msg, int fdclient);
		void send_privmsgs(std::vector<std::string> users, std::string message, int fdclient);

		/*			QUIT FUNCTIONS			*/
		void quit_cmd(std::string msg, int fdclient);

		/*			KICK FUNCTIONS			*/
		void kick_cmd(std::string msg, int fdclient);
		void kick_users(std::string channel_name, std::vector<std::string> users, std::string reason, int fdclient);

		/*			INVITE FUNCTIONS		*/
		void invite_cmd(std::string msg, int fdclient);

		/*			TOPIC FUNCTIONS			*/
		void topic_cmd(std::string msg, int fdclient);

		/*			PART FUNCTIONS			*/
		void part_cmd(std::string msg, int fdclient);
        void leave_the_channels(std::vector<std::string> channels, int fdclient, std::string reason);

        /*			MODE FUNCTIONS			*/
        void mode_cmd(std::string msg, int fdclient);
        void mode_invite(std::string channel_name, std::string mode);
        void mode_topic(std::string channel_name, std::string mode);
        void mode_password(std::string channel_name, std::string mode, std::string value, int fdclient);
        void mode_op(std::string channel_name, std::string mode, std::string value, int fdclient);
        void mode_limit(std::string channel_name, std::string mode, std::string value, int fdclient);
		

		/*			MPLAY FUNCTIONS			*/
		void mplay_cmd(std::string msg, int fdclient);
    
};

void msg_format(int fdclient, std::string cmd, std::string nick, std::string msg);
void ft_send(int fdclient, std::string msg);
std::string host();

#endif