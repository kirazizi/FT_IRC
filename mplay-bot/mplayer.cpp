/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mplayer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 21:52:04 by sbzizal           #+#    #+#             */
/*   Updated: 2024/03/11 14:43:59 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mplayer.hpp"

void send_to_server(int sockfd, std::string msg , std::string client) {
    std::stringstream ss;
    ss << msg << std::endl;
    std::string test = "PRIVMSG " + client + " :" + ss.str();
    send(sockfd, test.c_str(), test.size(), 0);
    for (int i = 0; i < 100000; i++);
    ss.str("");
    std::cout << msg << std::endl;
}

void print_mp3list(int sockfd, const std::vector<std::string> &mp3List, std::string client){
    
    send_to_server(sockfd, "Public Playlist: ", client);
    send_to_server(sockfd, "---------------------", client);
    for (int i = 0; i < (int)mp3List.size(); i++) {
        std::stringstream ss;
        ss << i + 1 << " - " << mp3List[i];
        send_to_server(sockfd, ss.str(), client);
    }
    send_to_server(sockfd, "---------------------", client);

}

void add_allmp3(const std::string &dirPath, std::vector<std::string> &mp3List) {
    DIR* directory = opendir(dirPath.c_str());
    if (directory) {
        dirent* entry;
        while ((entry = readdir(directory))) {
            if (std::strstr(entry->d_name, ".mp3")) {
                mp3List.push_back(entry->d_name);
            }
        }
        closedir(directory);
    }
}

void play_song(int sockfd, const std::vector<std::string> &mp3List, int index, std::string client) {
    if (index >= 0 && index < (int)mp3List.size()){
        system("killall afplay");
        system("clear");
        std::string command = "cd ./mplay-bot/songs && afplay \"" + mp3List[index] + "\" &";
        system(command.c_str());
        std::cout << "Now playing: " << mp3List[index] << std::endl;
        std::cout << "\033[1;32m ♬ the song is playing... \033[0m" << std::endl;
        
        // send message to client
        
        std::stringstream ss;
        ss << "Now playing: " << mp3List[index] << std::endl;
        std::string msg = "PRIVMSG " + client + " :" + ss.str();
        send(sockfd, msg.c_str(), msg.size(), 0);
    }
    else
        send_to_server(sockfd, "Invalid song number", client);

}

void play_random(int sockfd, const std::vector<std::string> &mp3List, std::string client) {
    system("killall afplay");
    system("clear");
    int index = std::rand() % mp3List.size();
    play_song(sockfd, mp3List, index, client);
}

void mplayer_cmd(int sockfd, std::stringstream &ss, std::vector<std::string> mp3List){
    std::string client;
    std::string cmd;
    std::string arg;
    ss >> client;
    ss >> cmd;
    ss >> arg;
    
    if (cmd == "list"){
        std::system("clear");
        print_mp3list(sockfd, mp3List, client);
    }
    if (cmd == "play"){
        if (arg != ""){
            int index = std::stoi(arg);
            play_song(sockfd, mp3List, index - 1, client);
        }
        else
            play_random(sockfd, mp3List, client);
    }
    else if (cmd == "stop"){
        system("killall afplay");
        std::system("clear");
        std::cout << "\033[1;31m ♬ the song is stopped... \033[0m" << std::endl;
        
        // send message to client
        std::string msg = "PRIVMSG " + client + " :The song is stopped\n";
        send(sockfd, msg.c_str(), msg.size(), 0);
    }
}

int ft_recv(int sockfd, std::vector<std::string> mp3List){    
    char buffer[1024];
    
    memset(buffer, 0, 1024);

    int rcv = recv(sockfd, buffer, 1024, 0);
    if (rcv < 0){
        system("killall afplay");
        system("clear");
        throw std::runtime_error("Error: Could not receive data from server");
    }
    else if (rcv == 0) {
        system("killall afplay");
        system("clear");
        throw std::runtime_error("Error: Server disconnected");;
    }

    std::stringstream ss(buffer);

    mplayer_cmd(sockfd, ss, mp3List);
    return 1;
}

int server_setup(std::string server_ip, int port){
    
    // Convert localhost
    if (server_ip == "localhost")
        server_ip = "127.0.0.1";

    // Create socket
    struct sockaddr_in server_addr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Error: Could not create socket");
    }

    // Fill in server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Error: Could not connect to server");
    }

    std::cout << "Connected to " << server_ip << ":" << port << std::endl;
    std::cout << "___________________________" << std::endl;
    
    std::string message;
    std::string bot = "BOT\n";
    
    send(sock, bot.c_str(), bot.size(), 0);

    return sock;
}

void signal_handler(int signum){
    (void)signum;
    system("killall afplay");
    system("clear");
    std::cout << "\033[1;31m Goodbye ... \033[0m" << std::endl;
    exit(1);
}

void mplayer::run(void){
    sockfd = server_setup(server_ip, port);
    if (sockfd < 0)
        throw std::runtime_error("Error: Could not connect to server");
    
    add_allmp3("./mplay-bot/songs", mp3List);

    while (true){
        if (ft_recv(sockfd, mp3List) != 1){
            break;
        }
    }

    close(sockfd);
    std::cout << "Disconnected\n";
}