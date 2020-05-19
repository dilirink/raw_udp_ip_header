//sudo tcpdump -i lo udp port 8081 -XX -vvv
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     8082
#define MAXLINE 1024 
  

int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char hello[] = "Hello from client ^-^"; 
    struct sockaddr_in     servaddr; 
  

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    int n, len; 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }      
    n = recvfrom(sockfd, (char *)buffer, sizeof(buffer),  0, (struct sockaddr *) &servaddr, 
                &len); 
    //buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
    len = sizeof(struct sockaddr_in);
    sendto(sockfd, hello, sizeof(hello), 0, (const struct sockaddr *) &servaddr,  len); 
    printf("Hello message sent.\n"); 

    close(sockfd); 
    return 0; 
} 