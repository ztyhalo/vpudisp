#include "hdmiapp.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <bitset>
#include <iostream>




sem_t                show_sem;
extern sem_t                dec_sem;
int                 jpg_size[IMG_BUF_SIZE];

#define TTL 64
#define BUFF_SIZE 1025


int rec_num = 0;


 char  imagedata[IMG_BUF_SIZE][IMG_FRAME_SIZE][1020];
 int  img_read;
 int  img_write;
uint   imageleng;
uint    imagetail;

HDMI_CLIENT::HDMI_CLIENT()
{
    hdmi_c_ttl = 0x924;
    hdmi_heart.udp_class_init(48689, "192.168.168.55");
    hdmi_heart.udp_broad_send_init(64);
    hdmi_heart.bind_port(48689);
    hdmi_image.udp_class_init(2068);
    hdmi_image.udp_mul_read_init("226.2.2.2");

//    hdmi_mark.udp_class_init(2067);
//    hdmi_mark.udp_mul_read_init("226.2.2.2");
}



void HDMI_CLIENT::hdmi_heart_ack(void)
{
    char send_buf[512] = {
            0x54, 0x46, 0x36, 0x7a, 0x60, 0x02, 0x00, 0x00,
            0x69, 0x00, 0x00, 0x03, 0x03, 0x01, 0x00, 0x26,
            0x1f, 0x1f, 0x00, 0x03, 0x36, 0x96, 0x87, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    send_buf[8] = hdmi_c_ttl;
    send_buf[9] = hdmi_c_ttl >> 8;
    hdmi_heart.udp_send(send_buf, 512);
    hdmi_c_ttl++;

}

void * hdmi_heart_pthread(void * arg)
{
    HDMI_CLIENT * client = (HDMI_CLIENT *)arg;
    char rev_buf[512];

    memset(rev_buf, 0x00, sizeof(rev_buf));

    struct sockaddr_in recaddr;
    memset(&recaddr, 0x00, sizeof(recaddr));

    while(1)
    {
        if(client->hdmi_heart.udp_read(rev_buf, 512, &recaddr))
        {
            if(inet_addr("192.168.168.56") == recaddr.sin_addr.s_addr)
            {
                printf("local\n");
            }
            else
            {
               client->hdmi_heart_ack();
            }
        }

    }
    return NULL;
}

#if 0
void * hdmi_image_progress(void * arg)
{

    //每次读到的字符串长度
    int str_len;
    int start_mark = 0;
    int ttl = 0;
    int end_leng = 0;



    //用于保存数据的字符缓冲
    uint8_t  buff[BUFF_SIZE];
    std::bitset<IMG_FRAME_SIZE> frame_mark;

    HDMI_CLIENT * client = (HDMI_CLIENT *)arg;
    sem_init(&show_sem, 0, 0); //zty2018

    memset(buff, 0x00, sizeof(buff));
    memset(imagedata, 0x00, sizeof(imagedata));
    printf("image start!");

    frame_mark.reset();
     img_read = 0;
     img_write = 0;

    while(1){
        //接收发送来的消息，因为之前已经将socket注册到组播中

            str_len= client->hdmi_image.udp_read(buff,1024);
            if(str_len<0){
                printf("read error!");
                break;
            }
            if(start_mark == 0 && buff[3] == 0)
            {
                start_mark = 1;
                rec_num = 1;
                ttl = 1;
                frame_mark.reset();
                frame_mark.set(0);
                memcpy(imagedata[img_write][0], buff+4,  str_len-4);

            }
            else if(start_mark)
            {

                ttl = frame_mark.test(buff[3]) ? (buff[3]+256) : buff[3];
                if(ttl >= IMG_FRAME_SIZE){
                    printf("img buf over\n");
                    start_mark = 0;
                }
                else{
                    frame_mark.set(ttl);
                    memcpy(imagedata[img_write][ttl], buff+4,  str_len-4);

                    if(buff[2] != 0)  //结束
                    {
                        start_mark = 0;
                        if(frame_mark.count() == ttl +1)
                        {
                            for( end_leng = 0; end_leng < str_len; end_leng++)
                            {
                                if(buff[4+end_leng] == 0xff && buff[4+end_leng+1] == 0xd9)
                                {
                                    break;
                                }
                            }

                            imageleng = ttl;
                            imagetail = end_leng +2;
                            jpg_size[img_write] = imageleng*1020+imagetail;
                            img_write++;
                            img_write %= IMG_BUF_SIZE;
                            if(img_write == img_read){
                                printf("img buf over img_write %d %d \n", img_write, jpg_size[img_write]);
                            }else{
                            sem_post(&show_sem);
                            }
                        }
                        else
                        {
                            printf("ttl error cout %d buff %d\n", frame_mark.count(),  ttl+1);
                        }
                    }
                }

            }
            else
                rec_num = 0;
        }


return 0;
}

#else
void * hdmi_image_progress(void * arg)
{

    //每次读到的字符串长度
    int str_len;
    int start_mark = 0;
    int ttl = 0;
    int end_leng = 0;



    //用于保存数据的字符缓冲
    uint8_t  buff[BUFF_SIZE];
    std::bitset<IMG_FRAME_SIZE> frame_mark;

    HDMI_CLIENT * client = (HDMI_CLIENT *)arg;
//    sem_init(&show_sem, 0, 0); //zty2018

    memset(buff, 0x00, sizeof(buff));
    memset(imagedata, 0x00, sizeof(imagedata));
    printf("image start!");

    frame_mark.reset();
     img_read = 0;
     img_write = 0;

    while(1){
        //接收发送来的消息，因为之前已经将socket注册到组播中

            str_len= client->hdmi_image.udp_read(buff,1024);
            if(str_len != 1024){
                printf("read error!");
                continue;
//                break;
            }
            if( buff[2] == 0 && buff[3] == 0)   //新的一帧
            {
                if(start_mark == 0){            //刚刚开始
                    start_mark = 1;
                    ttl = 1;
                    frame_mark.reset();
                    frame_mark.set(0);
                    memcpy(imagedata[img_write][0], buff+4,  str_len-4);
                }

            }
            else if(start_mark)
            {
                if(buff[2] >> 7)
                    ttl = ((buff[2] -128) << 8) + buff[3];
                else
                    ttl= (buff[2] << 8) + buff[3];

                if(ttl >= IMG_FRAME_SIZE){
                    printf("img buf over %d %d %d\n", ttl, buff[2], buff[3]);
                    ttl = 0;
                    start_mark = 0;
                }else{
                    frame_mark.set(ttl);
                    memcpy(imagedata[img_write][ttl], buff+4,  str_len-4);
                }

                if(buff[2] >> 7){    // 结束

                    if(frame_mark.count() == ttl +1)
                    {
                        for( end_leng = 0; end_leng < str_len; end_leng++)
                        {
                            if(buff[4+end_leng] == 0xff && buff[4+end_leng+1] == 0xd9)
                            {
                                break;
                            }
                        }

                        imageleng = ttl;
                        imagetail = end_leng +2;
                        jpg_size[img_write] = imageleng*1020+imagetail;
                        img_write++;
                        img_write %= IMG_BUF_SIZE;
                        if(img_write == img_read){

                            printf("img buf over img_write %d %d \n", img_write, jpg_size[img_write]);
                        }else{
//                            std::cout << "post!" << std::endl;
                        sem_post(&show_sem);
                        }
                    }
                    else
                    {
                      printf("ttl error cout %d buff %d\n", frame_mark.count(),  ttl+1);

                    }
                    ttl = 0;
                    start_mark = 0;
                }
            }else{
                ttl = 0;
                start_mark = 0;
            }
        }


return 0;
}
#endif
void error_handling(char * message){
    fputs(message,stderr);
    fputc('\n',stderr);
    return;
}

