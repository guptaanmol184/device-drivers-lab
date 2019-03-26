#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <string.h>

void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
    );

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);

int main(int argc, char *argv[])
{

    char *dev;
    /* char dev[10]; */
    char errbuf[PCAP_ERRBUF_SIZE];

    // find a device to sniff on
    /* printf("enter device name: "); */
    /* scanf("%s", dev); */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return(2);
    }


    // get device info
    char ip[13], subnet_mask[13];
    bpf_u_int32 ip_raw, subnet_mask_raw;
    struct in_addr address;
    if (pcap_lookupnet(dev, &ip_raw, &subnet_mask_raw, errbuf) == -1)
    {
        printf("pcap_lookup err: %s", errbuf);
        return(2);
    }

    /*
    If you call inet_ntoa() more than once
    you will overwrite the buffer. If we only stored
    the pointer to the string returned by inet_ntoa(),
    and then we call it again later for the subnet mask,
    our first pointer (ip address) will actually have
    the contents of the subnet mask. That is why we are
    using a string copy to grab the contents while it is fresh.
    The pointer returned by inet_ntoa() is always the same.

    This is from the man:
    The inet_ntoa() function converts the Internet host address in,
    given in network byte order, to a string in IPv4 dotted-decimal
    notation. The string is returned in a statically allocated
    buffer, which subsequent calls will overwrite.
    */

    /* Get ip in human readable form */
    address.s_addr = ip_raw;
    strcpy(ip, inet_ntoa(address));
    if (ip == NULL) {
        perror("inet_ntoa"); /* print error */
        return 1;
    }

    /* Get subnet mask in human readable form */
    address.s_addr = subnet_mask_raw;
    strcpy(subnet_mask, inet_ntoa(address));
    if (subnet_mask == NULL) {
        perror("inet_ntoa");
        return 1;
    }

    printf("Device: %s\n", dev);
    printf("IP address: %s\n", ip);
    printf("Subnet mask: %s\n", subnet_mask);


    // opening a device for live capture
    pcap_t *handle;
	const u_char *packet;
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        printf("error opening handle");
        return(2);
    }
    /* printf("The header type = %d\n", pcap_datalink(handle)); */
    /* Attempt to capture one packet. If there is no network traffic
      and the timeout is reached, it will return NULL */
    /* packet = pcap_next(handle, &packet_header); */
    /* if (packet == NULL) */
    /* { */
    /*     printf("No packet found.\n"); */
    /*     return 2; */
    /* } */

    /* /1* Our function to output some info *1/ */
    /* printf("%d. ", i); */
    /* print_packet_info(packet, packet_header); */

    // process all incomming packets one by one
    pcap_loop(handle, 0, my_packet_handler, NULL);

    return(0);
}



void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr* header,
    const u_char* packet
) {
    struct ether_header *eth_header;
    /* The packet is larger than the ether_header struct,
       but we just want to look at the first part of the packet
       that contains the header. We force the compiler
       to treat the pointer to the packet as just a pointer
       to the ether_header. The data payload of the packet comes
       after the headers. Different packet types have different header
       lengths though, but the ethernet header is always the same (14 bytes) */
    eth_header = (struct ether_header *) packet;

    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        printf("IP\n");
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
        printf("ARP\n");
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_REVARP) {
        printf("Reverse ARP\n");
    }

    static int i = 0;
    printf("%d", i);
    i++;
}

/*
void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *packet_header,
    const u_char *packet_body
    )
{
    print_packet_info(packet_body, *packet_header);
    return;
}
*/

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}
