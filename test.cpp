#include <iostream>
#include <sys/select.h>

// while (true)
 // {
 //     int i = poll(&vpoll[0], vpoll.size(), 0);
 //     if (i < 0)
 //     {
 //         std::cout << "Error: poll" << std::endl;
 //         exit(1);
 //     }
 //     else if (i == 0)
 //     {
 //         continue;
 //     }
 //     // accept client func
 //     for (unsigned long j = 0; j < vpoll.size(); j++)
 //     {
 //         if (vpoll[j].revents & POLLIN)
 //         {
 //             if (vpoll[j].fd == fdsocket)
 //             {
 //                 int fdclient;
 //                 struct sockaddr_in client;
 //                 socklen_t client_size = sizeof(client);
 //                 fdclient = accept(fdsocket, (struct sockaddr *)&client, &client_size);
 //                 if (fdclient == -1){
 //                     std::cout << "Error: accepting connection" << std::endl;
 //                     exit(1);
 //                 }
 //                 fcntl(fdclient, F_SETFL, O_NONBLOCK); 
 //                 std::cout << "Connection accepted" << std::endl;
 //                 clientfds.push_back(fdclient);
 //                 struct pollfd clientpoll;
 //                 clientpoll.fd = fdclient;
 //                 clientpoll.events = POLLIN;
 //                 clientpoll.revents = 0;
 //                 vpoll.push_back(clientpoll);
 //             }
 //             else
 //             {
 //                 char buffer[1024];
 //                 memset(buffer, 0, 1024);
 //                 int n = recv(vpoll[j].fd, buffer, 1024, 0);
 //                 if (n < 0)
 //                 {
 //                     std::cout << "Error: reading from socket" << std::endl;
 //                     exit(1);
 //                 }
 //                 if (n == 0)
 //                 {
 //                     std::cout << "Client disconnected" << std::endl;
 //                 }
 //                 std:: cout << "Client: " << buffer << std::endl;
 //                 // recive or send data from client
 //             }
 //         }
 //     }
 // }


int main(){
    int v = 1041;
    int h = 0x411;
    char *p = (char*)&v;

    // std::cout << "value: "<< h << std::endl;

    // printf("address: %p\n", v);
    printf("first: %x\n", *p);
    p++;
    printf("second: %x\n", *p);
    p++;
    printf("third: %x\n", *p);

    printf("FD_SETSIZE: %d\n", FD_SETSIZE);
}