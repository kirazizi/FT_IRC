/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 13:21:55 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/11 13:56:06 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/headers.hpp"
#include "./server/server.hpp"

int main(int ac, char **av){
    int fdsocket;
    if (ac != 3){
        std::cout << "Usage: ./server <port> <password>" << std::endl;
        exit(1);
    }

    std::string password = av[2];
    int port = std::atoi(av[1]);

    parsing_input(port, password);
    server srv(port, password);
    fdsocket = srv.server_setup();
    srv.server_polling(fdsocket); 

    return 0;
}