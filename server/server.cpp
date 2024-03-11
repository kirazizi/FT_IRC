/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:01:24 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/11 14:42:18 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

int server::server_setup(){
    std::cout << "█░█░█ █▀▀ █░░ █▀▀ █▀█ █▀▄▀█ █▀▀   ▀█▀ █▀█   █ █▀█ █▀▀   █▀ █▀▀ █▀█ █░█ █▀▀ █▀█" << std::endl;
    std::cout << "▀▄▀▄▀ ██▄ █▄▄ █▄▄ █▄█ █░▀░█ ██▄   ░█░ █▄█   █ █▀▄ █▄▄   ▄█ ██▄ █▀▄ ▀▄▀ ██▄ █▀▄" << std::endl;
    
    std::cout << "\033[34m\t\tServer is running on port: " << this->port << "\033[0m"<< std::endl;
    std::cout << "\033[34m\t\t     host: " << host() << "\033[0m"<< std::endl;

    // create socket using socket() function
    int fdsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (fdsocket < 0){
        std::cout << "Error: creating socket" << std::endl;
        exit(1);
    }
    
    // set socket to non-blocking using fcntl() function
    fcntl(fdsocket, F_SETFL, O_NONBLOCK);
    
    // set socket options using setsockopt() function to reuse the address
    int yes = 1;
    if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(int)) < 0){
        std::cout << "Error: setsockopt failed" << std::endl;
        exit(1);
    }
    
    struct sockaddr_in srv;
    // clear address structure
    memset(&srv, 0, sizeof(srv));
    
    // set address structure
    srv.sin_family = AF_INET;
    srv.sin_port = htons(this->port);
    srv.sin_addr.s_addr = INADDR_ANY;

    // bind socket to the server address using bind() function
    if(bind(fdsocket, (struct sockaddr *)&srv, sizeof(srv)) < 0){ // bind socket to the server address
        std::cout << "Error: binding socket" << std::endl;
        exit(1);
    }
    
    // listen for connections using listen() function
    if(listen(fdsocket, SOMAXCONN) < 0){
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
    struct pollfd clpoll;
    clpoll.fd = fdclient;
    clpoll.events = POLLIN;
    clpoll.revents = 0;
    vpoll.push_back(clpoll);
    
    class client obj_client(fdclient);
    obj_client.client_ip = get_ip(client.sin_addr);
    vec_clients.push_back(obj_client);
    std::cout<< "\033[33mclient: " << obj_client.client_ip << " connecting ...\033[0m" << std::endl;
}

void server::identify_client(std::string msg,int fdclient){
    std::stringstream ss(msg);
    client *target = NULL;
    std::string cmd;
    std::string value;
    std::string param;
    ss >> cmd;
    ss >> value;
    ss >> param;
    
    cmd = to_upper(cmd);
    for (size_t i = 0; i < vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient)
            target = &vec_clients[i];
    if (target == NULL)
        return;
    if (target->is_connected){
        handle_cmd(msg, fdclient);
        return;
    }
    if (cmd == "BOT"){
        target->bot = cmd; // set bot
        target->nickname = cmd;
        std::cout << "\033[32m" << target->nickname << " has joined" << "\033[0m" << std::endl;
        target->is_connected = 1;
        map_clients[target->nickname] = *target;
        ss.clear();
        return;
    }
    if(cmd == "USER" && value != ""){
        int space = value.find(' ');
        if (value.find(' ') != std::string::npos)
            value = value.substr(0, space);
        target->username = value;
    }
    if(cmd == "PASS" && value != ""){
        // check password
        if (this->srv_pass != value){
            msg_format(fdclient, "464", value , "Wrong password please try again!");
            ss.clear();
            return ;
        }
        target->password = value;
    }
    if(cmd == "NICK" && value != ""){
        // nick name policy
        if (nick_policy(value, fdclient)){
            ss.clear();
            return ;
        }
        // check if nickname is already taken
        for (size_t i = 0; i < vec_clients.size(); i++){
            if (vec_clients[i].nickname == value){
                msg_format(fdclient, "433", "+_+" , "Nickname already taken please try again!");
                ss.clear();
                return ;
            }
        }
        // check if nickname contains space
        if (param != ""){
            msg_format(fdclient, "432", "+_+" , "Nickname cannot contain space please try again!");
            ss.clear();
            return ;
        }
        target->nickname = value;
    }
    // check if all required fields are filled
    if(!target->username.empty() && !target->nickname.empty() && !target->password.empty()){
        if(!target->is_connected){
            std::cout << "\033[32m" << target->nickname << " has joined" << "\033[0m" << std::endl;
            msg_format(fdclient, "001", target->nickname, "Welcome to chat server");
            std::string motd = "your host is " + host() + ", running version 1.2";
            msg_format(fdclient, "002", target->nickname, motd);
            target->is_connected = 1;
            map_clients[target->nickname] = *target;
            ss.clear();
            return;
        }
    }
    ss.clear();
}

int server::server_recieve(int fdclient){
    char msg[1024];
    memset(msg, 0, 1024);
    int rcv = recv(fdclient, msg, 1023, 0);
    if (rcv < 0){
        std::cout << "Error: reading from socket" << std::endl;
        return 1;
    }

    if (rcv == 0){
        return 1;
    }

    for (size_t i = 0; i < vec_clients.size(); i++){
        if (vec_clients[i].fd == fdclient){
            vec_clients[i].buffer_cl += msg;
            if (vec_clients[i].buffer_cl.find("\n") == std::string::npos)
                return 0;
            std::string msg = vec_clients[i].buffer_cl;
            this->vec_clients[i].buffer_cl.clear();
            identify_client(msg, fdclient);
        }
    }
    return 0;
}

void server::server_polling(int fdsocket){
    signal(SIGPIPE, SIG_IGN);
    struct pollfd srvpoll;

    srvpoll.fd = fdsocket;
    srvpoll.events = POLLIN;
    srvpoll.revents = 0;

    vpoll.push_back(srvpoll);
    
    while(true){
        int pl = poll(&vpoll[0], vpoll.size(), 0);
        if (pl == -1){
            std::cout<< "Error: poll" << std::endl;
            // clear and close all sockets
            clear_all_client();
        }
        if(pl == 0)
            continue;
        for(int i = 0; i < (int)vpoll.size(); i++){
            if(vpoll[i].revents & POLLIN){
                if(vpoll[i].fd == fdsocket) // accept new connection
                    server_accept(fdsocket);
                else                        // recieve data from client
                    if(server_recieve(vpoll[i].fd)){
                        quit_cmd("Leaving...", vpoll[i].fd);
                        break;
                    }
            }
        }
    }
}
