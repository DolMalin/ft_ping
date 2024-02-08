#include "../includes/main.h"


struct packet {
    struct icmphdr icmp_header;
    char msg[BUFFER_SIZE]; // todo
};

struct packet_r {
    struct iphdr ip_header;
    struct icmphdr icmp_header;
    char msg[BUFFER_SIZE]; // todo  
};

uint16_t in_cksum(uint16_t *addr, int len)
{

    register long sum = 0;
    size_t count = len;

    while( count > 1 )  {
            sum += * (unsigned short *) addr++;
            count -= 2;
    }

    if( count > 0 )
            sum += * (unsigned char *) addr;

    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}


int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    // Creating raw socket
    int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd == -1) {
        printf("cannot create socket");
        return 1;
    }

    struct packet packet;
    ft_memset(&packet, 0, sizeof(packet));

    // Setup addr
    struct sockaddr_in source_addr, dest_addr;
    source_addr.sin_family = AF_INET;
    dest_addr.sin_family = AF_INET;

    inet_pton(AF_INET, av[1], &source_addr.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr.s_addr);

    // Setup icmp header
    packet.icmp_header.type = ICMP_ECHO;
    packet.icmp_header.code = 0;
    packet.icmp_header.un.echo.id = ID;
    packet.icmp_header.un.echo.sequence = 5;
    packet.icmp_header.checksum = 0;
    packet.icmp_header.checksum = in_cksum((unsigned short *)&packet, sizeof(packet));


    // Send packet
    int ret = sendto(socket_fd, &packet, sizeof(struct packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (ret == -1)
        printf("err send\n");

    struct msghdr msg;
    struct iovec iov;
    char buffer[BUFFER_SIZE];

    ft_memset(&msg, 0, sizeof(msg));
    ft_memset(&iov, 0, sizeof(iov));

    iov.iov_base = buffer;
    iov.iov_len = BUFFER_SIZE;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int i = 10;
    while (i--)
    {
        int ret_msg = recvmsg(socket_fd, &msg, 0);
        if (ret_msg == -1)
            printf("err recv\n");
        
        struct packet_r *res = (struct packet_r *)msg.msg_iov->iov_base;
        
        printf("%d\n", res->icmp_header.un.echo.sequence);
        
        exit(1);
    }
    


}