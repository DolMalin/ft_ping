#include "../includes/main.h"


struct packet {
    struct icmphdr icmp_header;
    char msg[BUFFER_SIZE - sizeof(struct icmphdr)]; // todo
};

struct packet_r {
    struct iphdr ip_header;
    struct icmphdr icmp_header;
    char msg[BUFFER_SIZE - sizeof(struct iphdr) - sizeof(struct icmphdr)]; // todo  
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

    inet_pton(AF_INET, "127.0.0.1", &source_addr.sin_addr.s_addr);
    inet_pton(AF_INET, av[1], &dest_addr.sin_addr.s_addr);

    // Setup icmp header
    packet.icmp_header.type = ICMP_ECHO;
    packet.icmp_header.code = 0;
    packet.icmp_header.un.echo.id = ID;
    packet.icmp_header.un.echo.sequence = 0;
    packet.icmp_header.checksum = 0;
    packet.icmp_header.checksum = in_cksum((unsigned short *)&packet, sizeof(packet));


    // Send/receive loop
    while (1)
    {
        struct timeval stop, start;
        ++packet.icmp_header.un.echo.sequence;

        gettimeofday(&start, NULL);
        int ret = sendto(socket_fd, &packet, sizeof(struct packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (ret == -1)
            printf("err send\n");

        struct msghdr msg;
        struct iovec iov;
        struct sockaddr_in addr;
        char buffer[BUFFER_SIZE];

        ft_memset(&msg, 0, sizeof(msg));
        ft_memset(&iov, 0, sizeof(iov));


        iov.iov_base = buffer;
        iov.iov_len = BUFFER_SIZE;
        msg.msg_name = &addr;
        msg.msg_namelen = sizeof(addr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;


        recvmsg(socket_fd, &msg, 0);
        gettimeofday(&stop, NULL);

        
        struct packet_r *res = (struct packet_r *)msg.msg_iov->iov_base;

        char dst[16] = "";
        inet_ntop(AF_INET, &addr.sin_addr, dst, sizeof(dst));

        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
            sizeof(*res),
            dst,
            res->icmp_header.un.echo.sequence,
            res->ip_header.ttl,
            ((stop.tv_sec * 1000000  + stop.tv_usec) -
            (start.tv_sec * 1000000  + start.tv_usec)) / 1000.0
        );

        usleep(1000000);
    }
    


}