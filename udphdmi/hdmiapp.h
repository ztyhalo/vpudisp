#ifndef HDMIAPP
#define HDMIAPP

//#include <QObject>
#include "udpsocket/udp.h"

#define IMG_BUF_SIZE 6
#define IMG_FRAME_SIZE 768

class OUDP_CLASS
{
public:
    OUDP_CLASS();



};
class HDMI_CLIENT
{

public:
    UDP_BROAD_CLASS       hdmi_heart;
    UDP_MULTI_CLASS       hdmi_image;
//    UDP_MULTI_CLASS       hdmi_mark;
    uint16_t              hdmi_c_ttl;
public:

    HDMI_CLIENT();

    void hdmi_heart_ack(void);


};

void * hdmi_heart_pthread(void * arg);
void * hdmi_image_progress(void * arg);


#endif // HDMIAPP

