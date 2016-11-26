#ifndef labCom_h
#define labCom_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace communication {
    // an die MFCs werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class LabCom : public Thread {
    public:
        //Defaultconstructor
        LabCom();
        //Destructor
        ~LabCom();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        bool reading;
        bool sending;

        char inDataBuffer[256];
        char singleCharBuffer;
        int bufferCharIndex;
    };
}

#endif
