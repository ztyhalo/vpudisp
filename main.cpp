//#include <QCoreApplication>
#include <stdio.h>
#include <string.h>
extern "C"
{
#include "vpu_lib.h"
#include "a9display/vpu_test.h"
}
#include "udphdmi/hdmiapp.h"

//#define ZTY_TEST
extern char  imagedata[300][1020];

struct cmd_line gA9display;

int main(int argc, char *argv[])
{
    pthread_t pid;
    pthread_t heart_pid;

    HDMI_CLIENT hdmi_c;



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
#endif
    decode_test(&gA9display);
    close_files(&gA9display);
     vpu_UnInit();

}
