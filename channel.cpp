/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:07:56 by tajjid            #+#    #+#             */
/*   Updated: 2024/02/06 15:41:27 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

channel::channel(std::string name) : name(name) {
	this->is_private = false;
	this->is_invite_only = false;
	this->is_limited = false;
	this->topic_restrict = false;
}

channel::~channel() {}

void channel::add_invited_client(int fdclient) {
	this->invited_clients.push_back(fdclient);
}

void channel::remove_invited_client(int fdclient) {
	for(size_t i = 0; i < this->invited_clients.size(); i++) {
		if (this->invited_clients[i] == fdclient) {
			this->invited_clients.erase(this->invited_clients.begin() + i);
			return;
		}
	}
}

bool channel::is_invited_client(int fdclient) {
	for(size_t i = 0; i < this->invited_clients.size(); i++) {
		if (this->invited_clients[i] == fdclient)
			return true;
	}
	return false;
}

void channel::add_client(int fdclient, std::string client_nick) {
	this->clients.push_back(std::make_pair(fdclient, client_nick));
}

void channel::remove_client(int fdclient) {
	for(size_t i = 0; i < this->clients.size(); i++) {
		if (this->clients[i].first == fdclient) {
			this->clients.erase(this->clients.begin() + i);
			return;
		}
	}
}

bool channel::is_client(int fdclient) {
	for(size_t i = 0; i < this->clients.size(); i++) {
		if (this->clients[i].first == fdclient)
			return true;
	}
	return false;
}

void channel::add_op(int fdclient) {
	this->op_clients.push_back(fdclient);
}

void channel::remove_op(int fdclient) {
	for(size_t i = 0; i < this->op_clients.size(); i++) {
		if (this->op_clients[i] == fdclient) {
			this->op_clients.erase(this->op_clients.begin() + i);
			return;
		}
	}
}

bool channel::is_op(int fdclient) {
	for(size_t i = 0; i < this->op_clients.size(); i++) {
		if (this->op_clients[i] == fdclient)
			return true;
	}
	return false;
}
