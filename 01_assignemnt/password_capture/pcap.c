
#include <stdio.h>
#include <pcap.h>

int main(int argc, char *argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev = argv[1];

    printf("Device: %s\n", dev);

    pcap_t *handle;
    handle = pcap_open_live(dev, BUFSIZ, 0, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return(2);
    }

    return(0);

}
