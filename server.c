
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void printCurrentTime()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);
    printf("Current local time and date: %s", asctime(timeinfo));
}

int main(int argc, char const *argv[]) 
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int socket_fd, fileDescriptorOfTheAcceptedSocket, readData;
    char buffer[BUFFER_SIZE] = {0};
    char *ackMessage = "Ack from server.";
    int option = 1;
    
    // Creating socket file descriptor.
    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // attaching socket to the port 8080.
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT,
                   (char*)&option, sizeof(option)) < 0)
    {
        perror("set sock option");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // attaching socket to the port 8080.
    if (bind(socket_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    /* extract the first connection on the queue of pending connections, create a new socket with the same socket type protocol and address family as the specified socket, and allocate a new file descriptor for that socket.
     */
    if ((fileDescriptorOfTheAcceptedSocket = accept(socket_fd, (struct sockaddr *)&address,
                                                    (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    // reads data on a socket with descriptor and stores it in a buffer.
    readData = read(fileDescriptorOfTheAcceptedSocket, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    // clear buffer
    bzero(buffer, sizeof(buffer));
    send(fileDescriptorOfTheAcceptedSocket, ackMessage,strlen(ackMessage), 0);
    printCurrentTime();
    printf("ack sent from Server.\n");
    
    while (1)
    {
        readData = read(fileDescriptorOfTheAcceptedSocket, buffer, BUFFER_SIZE);
        printf("Client says: %s\n", buffer);
        // clear buffer
        bzero(buffer, sizeof(buffer));
    }
    return 0;
} 

