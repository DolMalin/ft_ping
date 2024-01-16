#include "../includes/main.h"


int main(int ac, char **av)
{
    // Creating raw socket
    int socket_fd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if (socket_fd == -1)
        return 1;

    //  Setup ip header
    char packet_buffer[PACKET_BUFFER_SIZE];
    ft_bzero(packet_buffer, PACKET_BUFFER_SIZE);
    struct iphdr *ip_header = (struct iphdr *)packet_buffer;

    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    // iph->tot_len // to setup
    ip_header->id = htons(59231);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr()

}