/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:07:56 by tajjid            #+#    #+#             */
/*   Updated: 2024/01/04 00:11:41 by tajjid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(std::string name) : name(name) {
	this->is_private = false;
	this->is_invite_only = false;
	this->is_limited = false;
	this->topic_restrict = false;
}

Channel::~Channel() {}

void Channel::add_invited_client(int fdclient) {
	this->invited_clients.push_back(fdclient);
}

void Channel::remove_invited_client(int fdclient) {
	for(size_t i = 0; i < this->invited_clients.size(); i++) {
		if (this->invited_clients[i] == fdclient) {
			this->invited_clients.erase(this->invited_clients.begin() + i);
			return;
		}
	}
}

bool Channel::is_invited_client(int fdclient) {
	for(size_t i = 0; i < this->invited_clients.size(); i++) {
		if (this->invited_clients[i] == fdclient)
			return true;
	}
	return false;
}

void Channel::add_client(int fdclient) {
	this->clients.push_back(fdclient);
}

void Channel::remove_client(int fdclient) {
	for(size_t i = 0; i < this->clients.size(); i++) {
		if (this->clients[i] == fdclient) {
			this->clients.erase(this->clients.begin() + i);
			return;
		}
	}
}

bool Channel::is_client(int fdclient) {
	for(size_t i = 0; i < this->clients.size(); i++) {
		if (this->clients[i] == fdclient)
			return true;
	}
	return false;
}

void Channel::add_op(int fdclient) {
	this->op_clients.push_back(fdclient);
}

void Channel::remove_op(int fdclient) {
	for(size_t i = 0; i < this->op_clients.size(); i++) {
		if (this->op_clients[i] == fdclient) {
			this->op_clients.erase(this->op_clients.begin() + i);
			return;
		}
	}
}

bool Channel::is_op(int fdclient) {
	for(size_t i = 0; i < this->op_clients.size(); i++) {
		if (this->op_clients[i] == fdclient)
			return true;
	}
	return false;
}
