// #include <iostream>
// #include <string>
// #include <vector>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <vector>
// #include <poll.h>
// #include <fcntl.h>

// int main(int ac, char **av)
// {
//     (void)ac;
//     // get port number
//     int port = std::atoi(av[1]);

//     // creat a socket
//     int fdsocket = socket(AF_INET, SOCK_STREAM, 0);
//     if(fdsocket == -1){
//         perror("socket");
//         exit(1);
//     }

//     // add struct data
//     struct sockaddr_in srv;

//     // clear addres
//     // memset(&srv, 0, sizeof(srv));

//     // init addr 
//     srv.sin_family = AF_INET;
//     srv.sin_port = htons(port);
//     srv.sin_addr.s_addr = INADDR_ANY;

//     // bind sokcet with port
//     if(bind(fdsocket, (struct sockaddr *)&srv, sizeof(srv))){
//         perror("bind");
//         exit(1);
//     }

//     // listen a sig of socket
//     if (listen(fdsocket, 10)){
//         perror("listen");
//         exit(1);
//     }

//     std::cout << "-------> server created port: " << port << std::endl;
//     // now a part of hundel server to accept multpl clients and polling

//     // vector for poll
//     std::vector<pollfd> vpoll;

//     // pollfd struct init
//     struct pollfd pollfd;
    
//     pollfd.fd = fdsocket;
//     pollfd.events = POLLIN;
//     pollfd.revents = 0;

//     vpoll.push_back(pollfd);

//     while(true){
//         // polling for new event
//         int pl = poll(&vpoll[0], vpoll.size(), 0);
//         if(pl == -1){
//             perror("poll");
//             exit(1);
//         }
//         if(pl == 0)
//             continue;
//         for(int i = 0; i < (int)vpoll.size(); i++){
//             if(vpoll[i].revents & POLLIN){
//                 if(vpoll[i].fd == fdsocket){
//                     // accept new client
//                     int fdclient = accept(fdsocket, NULL, NULL);
//                     if (fdclient == -1){
//                         perror("accept");
//                         exit(1);
//                     }
//                     std::cout << "new client connected :)" << std::endl;
//                     struct pollfd clpoll;
//                     clpoll.fd = fdclient;
//                     clpoll.events = POLLIN;
//                     clpoll.revents = 0;

//                     vpoll.push_back(clpoll);
//                 }
//                 else{
//                     // msg recive from client
//                     char message[1024];

//                     if(recv(vpoll[i].fd, message, 1024, 0) == -1)
//                         perror("recv");
//                     std::cout<< "client " << vpoll[i].fd << " send: " << message;
//                 }
//             }
//         }
//     }
//     return 0;
// }