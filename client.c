
#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void printCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);
    printf("Current local time and date: %s", asctime(timeinfo));
}

int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in serverAddress;
    char *ackMessage = "Ack from client.";
    char userMessage[100];
    int sock = 0, readData;
    const char* loopBackAddress = "127.0.0.1";
    char buffer[BUFFER_SIZE] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n error in creating Socket.\n");
        return -1;
    }
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form.
    if(inet_pton(AF_INET, loopBackAddress, &serverAddress.sin_addr)<=0)
    {
        printf("\n Invalid address. \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("\n Connection Failed. \n");
        return -1;
    }

    send(sock, ackMessage, strlen(ackMessage), 0);
    printCurrentTime();
    printf("ack sent from Client.\n");
    // reads data on a socket with descriptor and stores it in a buffer.
    readData = read(sock, buffer, BUFFER_SIZE);
    printf("%s\n",buffer);
    while (1) {
        printf("Server is ready to listen. Type your message:");
        fgets(userMessage, 100,stdin);
        send(sock, userMessage, strlen(userMessage), 0);
    }
    return 0;
} 

