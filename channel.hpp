/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 19:01:18 by tajjid            #+#    #+#             */
/*   Updated: 2024/01/03 22:57:45 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "server.hpp"
#include "client.hpp"

class server;

class Channel{
	public:
		std::string name;
		std::vector<int> clients;
		std::vector<int> op_clients;

		bool is_private;
		std::string password;
		
		bool is_invite_only;
		std::vector<int> invited_clients;
		
		bool is_limited;
		int limit;
		
		bool topic_restrict;
		std::string topic;
		
		Channel(std::string name);
		~Channel();

		void add_invited_client(int fdclient);
		void remove_invited_client(int fdclient);
		bool is_invited_client(int fdclient);
		
		void add_client(int fdclient);
		void remove_client(int fdclient);
		bool is_client(int fdclient);
		
		void add_op(int fdclient);
		void remove_op(int fdclient);
		bool is_op(int fdclient);
};

#endif