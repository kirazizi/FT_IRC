/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:01:24 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/08 13:13:26 by sbzizal          ###   ########.fr       */
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
    std::string cmd = get_cmd(msg);
    std::string value = get_value(msg);
    client *target = NULL;
    
    for (size_t i = 0; i < vec_clients.size(); i++)
        if (vec_clients[i].fd == fdclient)
            target = &vec_clients[i];
    if (target == NULL)
        return;
    if(cmd == "USER" && value != ""){
        int space = value.find(' ');
        if (value.find(' ') != std::string::npos)
            value = value.substr(0, space);
        target->username = value;
    }
    else if(cmd == "PASS" && value != ""){
        // check password
        if (this->srv_pass != value){
            send(fdclient, "Wrong password please try again!\n", 33, 0);
            return;
        }
        target->password = value;
    }
    else if(cmd == "NICK" && value != ""){
        // check if nickname is already taken
        for (size_t i = 0; i < vec_clients.size(); i++){
            if (vec_clients[i].nickname == value){
                send(fdclient, "Nickname already taken please try again!\n", 41, 0);
                return;
            }
        }
        if (value.find(' ') != std::string::npos){
            send(fdclient, "Nickname cannot contain space please try again!\n", 48, 0);
            return;
        }
        target->nickname = value;
    }
    if(!target->username.empty() && !target->nickname.empty() && !target->password.empty()){
        if(!target->is_connected){
            std::cout << target->username << " has joined" << std::endl;
            send(fdclient, "Welcome to chat server\n", 23, 0);
            target->is_connected = 1;
            // map_clients.insert(std::pair<int, client>(target->fd, *target));
            map_clients[target->nickname] = *target;
            return;
        }
        // std::cout << target->nickname << ": " << msg;
        handle_cmd(msg, fdclient);
    }    
}

void split_cmd(std::string &msg){
    // i want to split this buffer with /r/n
    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = msg.find(delimiter)) != std::string::npos){
        token = msg.substr(0, pos);
        msg.erase(0, pos + delimiter.length());
    }
}

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
    // std::cout << "Client: " << msg;
    // split_cmd(str_msg);
    // std::string str_msg = msg;
    // std::string new_msg;
    // for(int i = 0; i < (int)str_msg.length(); i++){
    //     if (str_msg[i] == '\r' || str_msg[i +1] == '\n'){
            
    // }
    // std::cout << "client: "<< new_msg << std::endl;
    // std::cout << "client: "<< str_msg << std::endl;
    identify_client(msg, fdclient);
    // for(int i = 0; i < (int)vec_clients.size(); i++)
    //     if (vec_clients[i].nickname != "")
    //         std::cout << vec_clients[i].nickname << std::endl;

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
