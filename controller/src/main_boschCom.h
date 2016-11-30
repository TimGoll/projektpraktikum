#ifndef MAIN_BOSCHCOM_H
#define MAIN_BOSCHCOM_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace communication {
    class Main_BoschCom : public Thread {
    public:
        //Defaultconstructor
        Main_BoschCom();
        //Destructor
        ~Main_BoschCom();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
    };
}

#endif
