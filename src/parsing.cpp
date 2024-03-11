/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:03:35 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/11 13:50:55 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

void password_policy(std::string password){
    // check password space
    if (password.find(' ') != std::string::npos){
        std::cout << "Error: password contains space" << std::endl;
        exit(1);
    }
    // check password length
    if (password.length() < 4){
        std::cout << "Error: password length must be at least 4 characters" << std::endl;
        exit(1);
    }
    if (password.length() > 16){
        std::cout << "Error: password length must be at most 16 characters" << std::endl;
        exit(1);
    }
}

void parsing_input(int port, std::string password){
    if (port < 1024 || port > 65535){
        std::cout << "Error: port number must be in the range 1024 to 65535" << std::endl;
        exit(1);
    }
    if (password.empty()){
        std::cout << "Error: password is empty" << std::endl;
        exit(1);
    }
    password_policy(password);
}