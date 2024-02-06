#include "../includes/main.h"


struct packet {
    struct iphdr ip_header;
    struct icmphdr icmp_header;
    char msg[256]; // todo
};


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

    //  Setup ip header
    packet.ip_header.ihl = 5;
    packet.ip_header.version = 4;
    packet.ip_header.tos = 0;
    packet.ip_header.id = htons(ID);
    packet.ip_header.frag_off = 0;
    packet.ip_header.ttl = 255;
    packet.ip_header.protocol = 0;
    packet.ip_header.check = 0;
    packet.ip_header.tot_len = htons(sizeof(packet));
    packet.ip_header.saddr = source_addr.sin_addr.s_addr;
    packet.ip_header.daddr = dest_addr.sin_addr.s_addr;

    // Setup icmp header
    packet.icmp_header.type = ICMP_ECHO;
    packet.icmp_header.code = 0;
    packet.icmp_header.checksum = 0;
    packet.icmp_header.un.echo.id = 12345;
    packet.icmp_header.un.echo.sequence = 0;


    packet.msg[0] = 'C';

    int ret = sendto(socket_fd, &packet, sizeof(struct packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (ret == -1)
        printf("err send\n");


}