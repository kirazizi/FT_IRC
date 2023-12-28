/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:01:24 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/28 22:18:33 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server(void){
}

server::server(int port, std::string password){
    this->port = port;
    this->password = password;
}

int server::server_setup(){
    
    std::cout << "------> Server is running on port :" << this->port << std::endl;

    // create socket using socket() function
    int fdsocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP
    if (fdsocket == -1){
        std::cout << "Error: creating socket" << std::endl;
        exit(1);
    }
    // fcntl(fdsocket, F_SETFL, O_NONBLOCK);

    
    // set socket options using setsockopt() function to reuse the address
    int yes = 1;
    if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(int)) == -1){
        std::cout << "Error: setsockopt failed" << std::endl;
        exit(1);
    }

    struct sockaddr_in srv;
    // clear address structure
    memset(&srv, 0, sizeof(srv));
    // bind socket to the server address using bind() function
    srv.sin_family = AF_INET; // IPv4
    srv.sin_port = htons(this->port); // convert port number to network byte order (big endian)
    srv.sin_addr.s_addr = INADDR_ANY; // IP address

    if(bind(fdsocket, (struct sockaddr *)&srv, sizeof(srv)) == -1){ // bind socket to the server address
        std::cout << "Error: binding socket" << std::endl;
        exit(1);
    }
    
    // listen for connections using listen() function
    if(listen(fdsocket, 10) == -1){ // 10 is the maximum number of connections in the queue
        std::cout << "Error: listening" << std::endl;
        exit(1);
    }

    return fdsocket;
}

void server::server_accept(int fdsocket){
    int fdclient;
    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
    fdclient = accept(fdsocket, (struct sockaddr *)&client, &client_size);
    if (fdclient < 0){
        std::cout << "Error: accepting connection" << std::endl;
        exit(1);
    }
    std::cout<< "connection accepted" << std::endl;
    struct pollfd clpoll;
    clpoll.fd = fdclient;
    clpoll.events = POLLIN;
    clpoll.revents = 0;
    vpoll.push_back(clpoll);
}

void server::server_recieve(int fdclient){
    char buffer[1024];
    memset(buffer, 0, 1024);
    int rcv = recv(fdclient, buffer, 1024, 0);
    if (rcv < 0){
        std::cout << "Error: reading from socket" << std::endl;
        exit(1);
    }
    if (rcv == 0){
        std::cout << "Client disconnected" << std::endl;
    }
    std:: cout << "Client "<< fdclient << " send: "<< buffer;
}

void server::server_polling(int fdsocket){
    
    struct pollfd srvpoll;
    
    srvpoll.fd = fdsocket;
    srvpoll.events = POLLIN;
    srvpoll.revents = 0;

    vpoll.push_back(srvpoll);

    while(true){
        int pl = poll(&vpoll[0], vpoll.size(), 0);
        if (pl < 0){
            std::cout<< "Error: poll" << std::endl;
            exit(1);
        }
        if(pl == 0)
            continue;
        for(int i = 0; i < (int)vpoll.size(); i++){
            if(vpoll[i].revents & POLLIN){
                if(vpoll[i].fd == fdsocket)
                    server_accept(fdsocket);
                else
                    server_recieve(vpoll[i].fd);
                    // recive or send data from client
            }
        }
    }
}
