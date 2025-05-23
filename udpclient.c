#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>  // Required for inet_addr()
#include <unistd.h>      // Required for close()

#define MAX 80
#define PORT 8088
#define SA struct sockaddr

int main()
{
    char buff[MAX];
    int sockfd, len, n;
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }

    bzero(&servaddr, sizeof(servaddr));  // Fix the size here to sizeof(servaddr)
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.1.1");  // Replace with server's IP address
    servaddr.sin_port = htons(PORT);
    len = sizeof(servaddr);

    for (;;) {
        printf("\nEnter message: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');  // Take input until newline
        sendto(sockfd, buff, n, 0, (SA *)&servaddr, len);  // Send n bytes

        bzero(buff, sizeof(buff));  // Clear buffer

        // Receive message from server
        if (recvfrom(sockfd, buff, sizeof(buff), 0, (SA *)&servaddr, &len) < 0) {
            perror("recvfrom failed");
            exit(1);
        }
        printf("From Server: %s\n", buff);

        if (strncmp("exit", buff, 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

    close(sockfd);  // Close socket
}

