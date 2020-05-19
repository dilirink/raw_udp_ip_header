//sudo tcpdump -i lo udp port 8081 -XX -vvv   sudo tcpdump udp port 8081 -XX -vvv ///  sudo tcpdump -i lo udp port 8082 -XX -vvv
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <netinet/udp.h>
#include <netinet/ip.h>


#define SOURCE_IP "127.0.0.1" //источник назначения
#define SOURCE_PORT 8081
#define DESTINATION_IP "127.0.0.1" //пункт назначение
#define DESTINATION_PORT 8082
#define SIZE 128
/*unsigned short csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}
*/ /// на будущие на сл работу точне)
int main(int argc, char *argv[])
{
    int sock;
    int flag=1;
    int len;
    int send_to;
    char *packet;
    char massage[100]="Hello client!";
    struct iphdr *ip_h;
    struct udphdr *udp_h;
    struct sockaddr_in servaddr;
    
    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(DESTINATION_PORT);
	servaddr.sin_addr.s_addr = inet_addr(DESTINATION_IP);

    packet = calloc(SIZE, sizeof(char));
    ip_h = (struct iphdr*)packet;
    udp_h = (struct udphdr*)(packet+sizeof(struct iphdr));

    ip_h->ihl = 5;
	  ip_h->version = 4;
	  ip_h->tos = 0;
    ip_h->tot_len = htons(sizeof(struct udphdr) + strlen(massage) + 1);
    ip_h->id = htons(11111);
    ip_h->frag_off = 0;
    ip_h->ttl=64;
    ip_h->protocol=IPPROTO_UDP; ///ну илил 17
    ip_h->saddr=inet_addr(SOURCE_IP);
    ip_h->daddr=inet_addr(DESTINATION_IP);
    ip_h->check=0;
    
    udp_h->source=htons(SOURCE_PORT);
    udp_h->dest=htons(DESTINATION_PORT);
    udp_h->len=htons(sizeof(struct udphdr)+sizeof(massage));
    udp_h->check=0;
    
    sock = socket (AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock == -1) 
    {
        perror("socket EROR ");
        exit(1);
    }
    
    if ((setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(int)))<0)
    {
        perror("setsockopt ");
        close(sock);
		exit(1);
    }

    memmove(packet + sizeof(struct iphdr) + sizeof(struct udphdr),massage,strlen(massage)+1 );

    len = sizeof(struct sockaddr_in);

    send_to = sendto(sock, packet, SIZE, 0, (struct sockaddr *)&servaddr, len);
	if (send_to < 0) 
    {
		perror("EROR sendto");
	} 
    else
    {
		printf("Send to= %d\n", send_to);
	}
   
}
