/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:01:24 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/31 16:11:06 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

    // send(fdclient, "please identify yourself\n", 25, 0);
    // send(fdclient, "USER <username>\n", 16, 0);
    // send(fdclient, "PASS <password>\n", 16, 0);
    // send(fdclient, "NICK <nickname>\n", 17, 0);

int server::server_setup(){
    std::cout << "█░█░█ █▀▀ █░░ █▀▀ █▀█ █▀▄▀█ █▀▀   ▀█▀ █▀█   █ █▀█ █▀▀   █▀ █▀▀ █▀█ █░█ █▀▀ █▀█" << std::endl;
    std::cout << "▀▄▀▄▀ ██▄ █▄▄ █▄▄ █▄█ █░▀░█ ██▄   ░█░ █▄█   █ █▀▄ █▄▄   ▄█ ██▄ █▀▄ ▀▄▀ ██▄ █▀▄" << std::endl; 
    
    std::cout << "\033[34m\t\tServer is running on port: " << this->port << "\033[0m" << std::endl;

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
    std::cout<< "\033[33mconecting ...\033[0m" << std::endl;
    struct pollfd clpoll;
    clpoll.fd = fdclient;
    clpoll.events = POLLIN;
    clpoll.revents = 0;
    vpoll.push_back(clpoll);

    class client obj_client(fdclient);
    vec_clients.push_back(obj_client);
}

void server::identify_client(std::string msg,int fdclient){
    std::string cmd = get_cmd(msg);;
    std::string value = get_value(msg);
    client *target = NULL;
    
    for (size_t i = 0; i < vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient)
            target = &vec_clients[i];
    if (target == NULL)
        return;
    if(cmd == "USER" && value != ""){
        target->username = value;
    }
    else if(cmd == "PASS" && value != ""){
        target->password = value;
    }
    else if(cmd == "NICK" && value != ""){
        target->nickname = value;
    }
    if(!target->username.empty() && !target->nickname.empty() && !target->password.empty()){
        if(target->password == this->srv_pass){
            std::cout << target->username << " has joined" << std::endl;
            send(fdclient, "Welcome to chat server\n", 23, 0);
        }
        else
            send(fdclient, "Wrong password please try again!\n", 33, 0);
    }
}

// void hundel_cmd(std::string cmd, int fdclient){
//     int i = 0;
//     std::string commands[] = {"JOIN", "LEAVE", "MSG", "QUIT"};
//     while(i < 5){
//         if(cmd == commands[i])
//             break;
//         i++;
//     }
//     switch(i){
//         case 0:
//             // join_channel();
//         case 1:
//             // leave_channel();
//         case 2:
//             // send_msg();
//         case 3:
//             // quit();
//     }
// }

void server::server_recieve(int fdclient){
    char msg[1024];
    memset(msg, 0, 1024);
    int rcv = recv(fdclient, msg, 1024, 0);
    if (rcv < 0){
        std::cout << "Error: reading from socket" << std::endl;
        exit(1);
    }
    if (rcv == 0){
        std::cout << "Client disconnected" << std::endl;
        exit(1);
    }
    identify_client(msg, fdclient);
    for(int i = 0; i < (int)vec_clients.size(); i++)
        if (vec_clients[i].nickname != "")
            std::cout << vec_clients[i].nickname << std::endl;
    // hundel commands
    // void hundel_cmd(std::string msg, int fdclient);
}

void server::server_polling(int fdsocket){
    
    struct pollfd srvpoll;
    
    srvpoll.fd = fdsocket;
    srvpoll.events = POLLIN;
    srvpoll.revents = 0;

    vpoll.push_back(srvpoll);
    
    while(true){
        int pl = poll(&vpoll[0], vpoll.size(), 0);
        if (pl == -1){
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
            }
        }
    }
}
