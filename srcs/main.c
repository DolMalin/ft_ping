#include "../includes/main.h"


struct packet {
    struct icmphdr icmp_header;
    char msg[32]; // todo
};


int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    // Creating raw socket
    int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
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
    packet.icmp_header.checksum = 0;
    packet.icmp_header.un.echo.id = 333;
    packet.icmp_header.un.echo.sequence = 0;

    // Setup message
    // packet.msg[0] = 'C';

    int ret = sendto(socket_fd, &packet, sizeof(struct packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (ret == -1)
        printf("err send\n");


}