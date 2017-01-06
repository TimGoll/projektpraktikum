#ifndef MAIN_BOSCHCOM_H
#define MAIN_BOSCHCOM_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "ownlibs/serialCommunication.h"

namespace communication {
    class Main_BoschCom : public Thread {
    public:
        //Defaultconstructor
        Main_BoschCom();
        //Destructor
        ~Main_BoschCom();
        void setIntervall(int intervall);
        void start(unsigned long time);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int intervall;
        bool ready;
        unsigned long lastTime;
    };
}

#endif
