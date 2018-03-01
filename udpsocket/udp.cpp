#include "udpsocket/udp.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <fcntl.h>
//#include <QDebug>

int UDP_CLASS::udp_send(void * buf, size_t n)
{
    int len;
     if((len=sendto(socket_fd, buf, n,0,(struct sockaddr *)&addr,sizeof(struct sockaddr)))<0)
     {
         perror("udp send");
         return -1;
     }
     return len;
}

int UDP_CLASS::udp_read_init(void)
{
    if(bind(socket_fd,(struct sockaddr *)&addr,sizeof(addr)) == -1){
            perror("udp bind");
            return -1;
    }
    return 0;
}

int UDP_CLASS::udp_read(void * buf, size_t n)
{
    return recvfrom(socket_fd, buf , n, 0, NULL, 0);
}

int UDP_CLASS::udp_read(void * buf, size_t n, struct sockaddr_in * soure)
{
    int size = sizeof(struct sockaddr_in);
    return recvfrom(socket_fd, buf , n, 0, (struct sockaddr *)soure,(socklen_t *) &size);
}

void UDP_CLASS::bind_port(uint16_t port)
{
    localaddr.socket_add_init(port);
    bind(socket_fd, (struct sockaddr*)&(localaddr.addr), sizeof(struct sockaddr_in));
}



int UDP_MULTI_CLASS::udp_mul_read_init(char * group)
{
    int nRecvBuf=512*1024;//设置为32K

    if(udp_read_init() < 0)
    {
        printf("udp read init error");
        return -1;
    }
    join_addr.imr_multiaddr.s_addr = inet_addr(group);//要加入的多播组地址
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);//加入该组的套接字所属的主机IP地址
    //设置socket中的IP_ADD_MEMBERSHIP选项加入多播组
    if(setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void *)&join_addr, sizeof(join_addr)) == -1)
    {
        printf("mul set error");
        perror("mul set:");
    }
    if(setsockopt(socket_fd,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int)) == -1)
    {
        {
            printf("mul rec buf error");
            perror("mul rec buf:");
        }
    }

    return 0;
}
int UDP_MULTI_CLASS::udp_mul_read_init(char * group, char * localip)
{

    if(udp_read_init() < 0)
        return -1;
    join_addr.imr_multiaddr.s_addr = inet_addr(group);//要加入的多播组地址
    join_addr.imr_interface.s_addr = inet_addr(localip);//加入该组的套接字所属的主机IP地址
    //设置socket中的IP_ADD_MEMBERSHIP选项加入多播组
    setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void *)&join_addr, sizeof(join_addr));
    return 0;
}



void UDP_MULTI_CLASS::udp_mul_send_init(int time)
{
    live_time = time;
    setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_TTL,(void *)&live_time,sizeof(live_time));
}


void UDP_BROAD_CLASS::udp_broad_send_init(int time)
{
    live_time = time;
    opt_so_broadcast = 1;
    //设置socket可选项，因为默认生成的会阻止广播，所以要将可选项中的SO_BROADCAST标志置为1
    setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,(void *)&opt_so_broadcast,sizeof(opt_so_broadcast));
}













