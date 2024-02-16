/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:01:24 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/16 16:50:39 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

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
    
    // set socket to non-blocking using fcntl() function
    fcntl(fdsocket, F_SETFL, O_NONBLOCK);
    
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
    

    // this just a check part i dont need it ---------------------> please ignore it
    // --------------------------------------------------------------------------------
    // const char *ip = "192.168.1.120"; // example IP address
    // srv.sin_addr.s_addr = inet_addr(ip); // convert IP address to network byte order
    // --------------------------------------------------------------------------------


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

std::string host(){
    system("hostname > /tmp/host.txt");
    std::ifstream file("/tmp/host.txt");
    std::string host;
    std::getline(file, host);
    file.close();
    return host;
}

void ft_send(int fdclient, std::string msg){
    if (send(fdclient, msg.c_str(), msg.length(), 0) < 0){
        std::cout << "Error: sending message" << std::endl;
        exit(1);
    }
}

void msg_format(int fdclient, std::string cmd, std::string nick, std::string msg){
    std::string prefix = host();
    std::string response = ":" + prefix + " " + cmd + " " + nick + " :" + msg + "\r\n";
    ft_send(fdclient, response);
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
        // check if nickname is already taken
        for (size_t i = 0; i < vec_clients.size(); i++){
            if (vec_clients[i].nickname == value){
                msg_format(fdclient, "433", "+_+" , "Nickname already taken please try again!");
                ss.clear();
                return ;
            }
        }
        if (param != ""){
            msg_format(fdclient, "432", "+_+" , "Nickname cannot contain space please try again!");
            ss.clear();
            return ;
        }
        target->nickname = value;
    }
    if(!target->username.empty() && !target->nickname.empty() && !target->password.empty()){
        if(!target->is_connected){
            std::cout << "\033[32m" << target->nickname << " has joined" << "\033[0m" << std::endl;
            msg_format(fdclient, "001", target->nickname, "Welcome to chat server");
            msg_format(fdclient, "002", target->nickname, "Your host is e3r8p2.1337.ma, running version 1.2");
            target->is_connected = 1;
            map_clients[target->nickname] = *target;
    
            return;
        }
    }
    ss.clear();
}

int server::server_recieve(int fdclient){
    char msg[1024];
    memset(msg, 0, 1024);
    int rcv = recv(fdclient, msg, 1024, 0);
    if (rcv < 0){
        std::cout << "Error: reading from socket" << std::endl;
       return 1;
    }

    if (rcv == 0){
        return 1;
    }
    this->buffer += msg;
    if (buffer.find("\n") == std::string::npos)
        return 0;
    std::cout << buffer;
    identify_client(buffer, fdclient);
    buffer.clear();
    return 0;
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
                if(vpoll[i].fd == fdsocket) // new connection
                    server_accept(fdsocket);
                else                        // recieve data
                    if(server_recieve(vpoll[i].fd)){
                        quit_cmd("Leaving...", vpoll[i].fd);
                    }
            }
        }
    }
}
