/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 13:21:55 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/08 10:43:56 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "server.hpp"
#include "parsing.hpp"

int main(int ac, char **av){
    
    parsing parsing;
    std::string password;
    int fdsocket;
    // check arguments number : first for port and second for password
    if (ac != 3){
        std::cout << "Usage: ./server <port> <password>" << std::endl;
        exit(1);
    }
    if (av[2])
        password = av[2];

    int port = std::atoi(av[1]);

    // parsing input
    parsing.parsing_input(port, password);
    server srv(port, password);
    fdsocket = srv.server_setup();
    srv.server_polling(fdsocket);

    return 0;
}