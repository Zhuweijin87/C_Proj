#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>          /* ioctl command */
#include <netinet/if_ether.h>   /* ethhdr struct */
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <errno.h>

/* 需要在root用户下运行，否则权限限制 */
int errno;

int set_socket_pakcet()
{
    int     fd;
    
    fd = socket(AF_INET, SOCK_PACKET, htons(0x0003));
    if(fd == -1){
        fprintf(stderr, "fail to socket:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    int  ret;
    char *ethname = "eth0";
    struct ifreq ifr;
    strcpy(ifr.ifr_name, ethname);
    
    ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
    if(ret < 0){
        fprintf(stderr, "fail to ioctl SIOCGIFFLAGS: (%d:%s)\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    ifr.ifr_flags |= IFF_PROMISC;
    ret = ioctl(fd, SIOCSIFFLAGS, &ifr);
    if(ret < 0){
        fprintf(stderr, "fail to ioctl SIOCSIFFLAGS: (%d:%s)\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    return 0;
}

struct arp_packet {
    unsigned short ar_hrd;      /* 硬件类型 */
    unsigned short ar_pro;      /* 协议类型 */
    unsigned char  ar_hln;      /* 硬件地址类型 */
    unsigned char  ar_pln;      /* 协议地址类型 */
    unsigned short ar_op;       /* ARP操作码 */
    unsigned char  ar_sha[ETH_ALEN];    /* 发送方MAC地址 */
    unsigned char* ar_sip;      /* 发送方IP地址 */
    unsigned char  ar_tha[ETH_ALEN];    /* 目的MAC地址 */
    unsigned char* ar_tip;      /* 目的IP地址 */
};

int ARP_request()
{
    char    ef[ETH_FRAME_LEN];  /* 以太帧缓冲区 */
    struct  ethhdr*  peth;
    /* 目的以太网地址 */
    char    dest_eth[ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    /* 源以太网地址 */
    char    src_eth[ETH_ALEN] = {0x00, 0x0C, 0x29, 0x73, 0x9D, 0x15};
    
    int fd = socket(AF_INET, SOCK_PACKET, htons(0x0003));
    if(fd == -1){
        fprintf(stderr, "fail to socket:(%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    peth = (struct ethhdr*)ef;
    /*复制目 的以太网地址*/
    memcpy(peth->h_dest, dest_eth, ETH_ALEN);

    /*复制源以太网地址*/
    memcpy(peth->h_source, src_eth, ETH_ALEN);

    peth->h_proto = htons(0x0806);          /* 设置协议类型, 以太网0x0806 */
    
    struct arp_packet *arpsock;
    arpsock = (struct arp_packet*)ef + ETH_HLEN;
    
    arpsock->ar_hrd = htons(0x1);           /* arp 硬件类型 */
    arpsock->ar_pro = htons(0x0800);        /* 协议类型 */
    arpsock->ar_hln = 6;                    /* 硬件地址长度 */
    arpsock->ar_pln = 4;                    /* IP 地址长度 */
    
    memcpy(arpsock->ar_sha, src_eth, ETH_ALEN);
    arpsock->ar_sip = (unsigned char*)inet_addr("192.168.1.46");
    memcpy(arpsock->ar_tha, dest_eth, ETH_ALEN);
    arpsock->ar_tip = (unsigned char*)inet_addr("192.168.1.1");

    /* 发送 ARP 请求 8 次, 间隔 1s */
    int i = 0;
    for(i=0;i<8;i++){
        write(fd, ef, ETH_FRAME_LEN); /*发送*/
        sleep(1); /*等待 1s*/
    }
    close(fd);

    return 0;
}

int main()
{
    
    return ARP_request();
}
