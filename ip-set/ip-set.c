#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int main()
{
    unsigned char ip_address[15], interface[10];
    int fd;
    struct ifreq ifr;
    struct sockaddr_in *addr;

    /*Read IP Address*/
    printf("Enter Ip Address: ");
    scanf("%s",ip_address);

    /* Interrface to set IP on */
    printf("Enter network interface name: ");
    scanf("%s", interface);

    /*AF_INET - to define network interface IPv4*/
    /*Creating soket for it.*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;

    /*eth0 - define the ifr_name - port name
    where network attached.*/
    memcpy(ifr.ifr_name, interface, IFNAMSIZ-1);

    /*defining the sockaddr_in*/
    addr=(struct sockaddr_in *)&ifr.ifr_addr;

    /*convert ip address in correct format to write*/
    inet_pton(AF_INET,ip_address,&addr->sin_addr);

    /*Setting the Ip Address using ioctl*/
    ioctl(fd, SIOCSIFADDR, &ifr);
    /*closing fd*/
    close(fd);

    printf("IP Address updated sucessfully.\n");

    /*Getting the Ip Address after Updating.*/

    /*clear ip_address buffer with 0x20- space*/
    memset((unsigned char*)ip_address,0x20,15);
    ioctl(fd, SIOCGIFADDR, &ifr);
    /*Extracting Ip Address*/
    strcpy(ip_address,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    printf("Updated IP Address is: %s\n",ip_address);

    return 0;
}
