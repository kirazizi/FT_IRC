/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:01:24 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/09 15:00:07 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <fstream>

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

std::string host(){
    system("hostname > /tmp/host.txt");
    std::ifstream file("/tmp/host.txt");
    std::string host;
    std::getline(file, host);
    file.close();
    return host;
}

void ft_send(int fdclient, std::string msg){
    send(fdclient, msg.c_str(), msg.length(), 0);
}

void msg_format(int fdclient, std::string cmd, std::string nick, std::string msg){
    (void)nick;
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
    if(cmd == "USER" && value != ""){
        target->username = value;
    }
    else if(cmd == "PASS" && value != ""){
        // check password
        if (this->srv_pass != value){
            msg_format(fdclient, "464", value , "Wrong password please try again!");
            return ;
        }
        target->password = value;
    }
    else if(cmd == "NICK" && value != ""){
        // check if nickname is already taken
        for (size_t i = 0; i < vec_clients.size(); i++){
            if (vec_clients[i].nickname == value){
                msg_format(fdclient, "433", "+_+" , "Nickname already taken please try again!");
                return ;
            }
        }
        if (param != ""){
            msg_format(fdclient, "432", "+_+" , "Nickname cannot contain space please try again!");
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
        // handle_cmd(msg, fdclient);
    }
}

// void split_cmd(std::string &msg){
//     // i want to split this buffer with /r/n
//     std::string delimiter = "\r\n";
//     std::string delimiter2 = "\n";
//     size_t pos = 0;
//     std::string token;
//     while ((pos = msg.find(delimiter)) != std::string::npos) {
//         token = msg.substr(0, pos);
//         msg.erase(0, pos + delimiter.length());
//     }
//     while ((pos = msg.find(delimiter2)) != std::string::npos) {
//         token = msg.substr(0, pos);
//         msg.erase(0, pos + delimiter2.length());
//     }
// }

int server::server_recieve(int fdclient){
    char msg[1024];
    memset(msg, 0, 1024);
    int rcv = recv(fdclient, msg, 1024, 0);
    if (rcv < 0){
        std::cout << "Error: reading from socket" << std::endl;
        exit(1);
    }
    if (rcv == 0){
        std::cout << "\033[31m" << "disconnecing ..." << "\033[0m" << std::endl;
        return 1;
    }
    this->buffer += msg;
    if (buffer.find("\n") == std::string::npos)
        return 0;
    std::cout << buffer;
    if (vec_clients[fdclient].is_connected == 0){
        identify_client(buffer, fdclient);
    }
    handle_cmd(buffer, fdclient);
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
                if(vpoll[i].fd == fdsocket)
                    server_accept(fdsocket);
                else
                    if(server_recieve(vpoll[i].fd)){
                        vpoll.erase(vpoll.begin() + i);
                        vec_clients.erase(vec_clients.begin() + (i -1));
                    }
            }
        }
    }
}
