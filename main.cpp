//#include <QCoreApplication>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <iostream>
extern "C"
{
#include "vpu_lib.h"
#include "a9display/vpu_test.h"
}
#include "udphdmi/hdmiapp.h"

//#define ZTY_TEST

extern char  imagedata[IMG_BUF_SIZE][IMG_FRAME_SIZE][1020];
extern sem_t                show_sem;
struct cmd_line gA9display;
pthread_t pid;
pthread_t heart_pid;
pthread_t link_pid;

void * link_progress(void * arg)
{
    HDMI_CLIENT * client = (HDMI_CLIENT *)arg;
    int err;
    int link_status = 0;
    struct ifreq ifr;
    memset(&ifr, 0x00, sizeof(ifr));
    memcpy(ifr.ifr_name, "eth0", 5);

    while(1){
        err = ioctl(client->hdmi_image.socket_fd,  SIOCGIFFLAGS, &ifr);
        if(!err){
            if( ((ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING))){
//                std::cout << "run!" <<std::endl;
                if(link_status == 0){
                    link_status = 1;
                }else if(link_status == 2){
                    pthread_cancel(pid);
                    pthread_join(pid,NULL);
                    client->hdmi_image.close_fd();

                   client->hdmi_image.udp_class_init(2068);
                   client->hdmi_image.udp_mul_read_init("226.2.2.2");


                  if(pthread_create(&pid, NULL, hdmi_image_progress, arg) != 0)
                  {
                      printf("creat pthread failed!\n");
                  }
                    link_status = 1;
                }
            }else{
                std::cout << "down!" <<std::endl;
                if(link_status == 1)
                {
                    link_status = 2;
                }
            }
        }else{
            std::cout << "ioctl err!";
        }
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{


    HDMI_CLIENT hdmi_c;
    int reset_count = 0;
    sem_init(&show_sem, 0, 0); //zty2018

    while(1){
    memset(&gA9display, 0x00 ,sizeof(struct cmd_line));
    vpu_test_dbg_level = 0;
    ::framebuf_init();
    vpu_Init(NULL);
#ifdef ZTY_TEST
    sprintf(gA9display.input,"/opt/nsize.mjpg");
    gA9display.src_scheme = 1;
#else
    gA9display.src_scheme = PATH_BUF;
#endif
    gA9display.bs_mode = 1;
    gA9display.format = 7;
    gA9display.membuf = (char *)imagedata;

    open_files(&gA9display);

#ifndef ZTY_TEST

    if(pthread_create(&heart_pid, NULL, hdmi_heart_pthread, &hdmi_c) != 0)
    {
        printf("creat pthread failed!\n");
        return -1;
    }

    if(pthread_create(&pid, NULL, hdmi_image_progress, &hdmi_c) != 0)
    {
        printf("creat pthread failed!\n");
        return -1;
    }
    if(pthread_create(&link_pid, NULL, link_progress, &hdmi_c) != 0)
    {
        printf("creat pthread failed!\n");
        return -1;
    }
    if(reset_count !=0){
        hdmi_c.hdmi_image.udp_class_init(2068);
        hdmi_c.hdmi_image.udp_mul_read_init("226.2.2.2");
        reset_count = 0;
    }

#endif

    decode_test(&gA9display);

    close_files(&gA9display);
     vpu_UnInit();
#ifndef ZTY_TEST
      pthread_cancel(pid);
      pthread_join(pid,NULL);
      pthread_cancel(heart_pid);
      pthread_join(heart_pid,NULL);
      hdmi_c.hdmi_image.close_fd();
      reset_count++;
      sleep(1);
#endif
    }


}
