/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 19:01:18 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/15 20:28:30 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "server.hpp"
#include "client.hpp"

class server;

class channel{
	public:
		std::string name;
		std::vector<std::pair<int, std::string> > clients;
		std::vector<int> op_clients;

		bool is_private;
		std::string password;
		
		bool is_invite_only;
		std::vector<int> invited_clients;
		
		bool is_limited;
		int limit;
		
		bool topic_restrict;
		std::pair<std::string, std::string> topic;
		
		channel(){};
		channel(std::string name);
		~channel();

		void add_invited_client(int fdclient);
		void remove_invited_client(int fdclient);
		bool is_invited_client(int fdclient);
		
		void add_client(int fdclient, std::string client_nick);
		void remove_client(int fdclient);
		bool is_client(int fdclient);
		
		void add_op(int fdclient);
		void remove_op(int fdclient);
		bool is_op(int fdclient);
		std::string get_ops();

		void set_password(std::string password);
		void remove_password();
		bool is_password(std::string password);

		void set_limit(int limit);
		void remove_limit();

		std::string get_modes();
};

#endif