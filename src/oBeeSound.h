#ifndef _oBeeSound_h
#define _oBeeSound_h

#include "lib/NonBlockingRtttl.h"

class oBeeSound
{
    public:

    oBeeSound();

    int pin;

    void Begin(char * iSongBuffer);
    void Update();
    void Stop();
    bool IsPlaying();

    char *song_startup = (char *)"Intel:d=16,o=5,b=320:d,p,d,p,d,p,g,p,g,p,g,p,d,p,d,p,d,p,a,p,a,p,a,2p";
    char *song_notification1 = (char *)"Fido:d=16,o=6,b=800:f,4p,f,4p,f,4p,f,4p,c,4p,c,4p,c,4p,c,1p,1p,1p,1p";
    char *song_notification2 = (char *)"Fido:d=8,o=7,b=500:f,f6,f,f6,f,f6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,c6,c,2p";
    char *song_notification3 = (char *)"Knock:d=32,o=5,b=100:e,4p,e,p,e,8p,e,4p,e,8p,e,4p";
    char *song_alamr1 = (char *)"Urgent:d=8,o=6,b=320:c,e,d7,c,e,a#,c,e,a,c,e,g,c,e,a,c,e,a#,c,e,d7";
    char *song_alamr2 = (char *)"Triple:d=8,o=5,b=400:c,e,g,c,e,g,c,e,g,c6,e6,g6,c6,e6,g6,c6,e6,g6,c7,e7,g7,c7,e7,g7,c7,e7,g7";
    char *song_alamr3 = (char *)"initiald:d=4,o=5,b=125:8d,8d,8d6,16d,16d6,8d,8d,8d6,16d,16d6,8d,8d,8d6,16d,16d6,8d,8d,d6,8d,8d,8d6,16d,16d6,8d,8d,8d6,16d,16d6,8d,8d,8d6,16d,16d6,8d,8d,d6";
    char *song_error = (char *)"Mosaic:d=8,o=6,b=400:c,e,g,e,c,g,e,g,c,g,c,e,c,g,e,g,e,c,p,c5,e5,g5,e5,c5,g5,e5,g5,c5,g5,c5,e5,c5,g5,e5,g5,e5,c5";
    char *song_custom1 = (char *)"";
    char *song_custom2 = (char *)"";
    char *song_custom3 = (char *)"";

};


#endif
