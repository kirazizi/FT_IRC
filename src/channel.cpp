/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 20:07:56 by tajjid            #+#    #+#             */
/*   Updated: 2024/03/11 13:50:24 by sbzizal          ###   ########.fr       */
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

std::string channel::get_ops() {
	std::string ops = "";
	for(size_t i = 0; i < this->clients.size(); i++) {
		if (is_op(this->clients[i].first))
			ops += "\'" + this->clients[i].second + "\' ";
	}
	return ops;
}

void channel::set_password(std::string password) {
    this->password = password;
    this->is_private = true;
}

void channel::remove_password() {
    this->password = "";
    this->is_private = false;
}

bool channel::is_password(std::string password) {
    if (this->password == password)
        return true;
    return false;
}

void channel::set_limit(int limit) {
    this->limit = limit;
    this->is_limited = true;
}

void channel::remove_limit() {
    this->limit = 0;
    this->is_limited = false;
}

std::string channel::get_modes() {
    std::string modes = "+";
    if (this->is_private)
        modes += "k";
    if (this->is_invite_only)
        modes += "i";
    if (this->is_limited)
        modes += "l";
    if (this->topic_restrict)
        modes += "t";
    return modes;
}

std::string channel::get_clients_names() {
	std::string clients;

	for(size_t i = 0; i < this->clients.size(); i++) {
		if (is_op(this->clients[i].first))
			clients += "@" + this->clients[i].second + " ";
		else
			clients += this->clients[i].second + " ";
	}
	return clients;
}

void channel::send_channel_msg(std::string msg, int fdclient) {
	int client_fd;
	for(size_t i = 0; i < this->clients.size(); i++) {
		client_fd = this->clients[i].first;
		if (client_fd != fdclient)
			send(client_fd, msg.c_str(), msg.size(), 0);
	}
}