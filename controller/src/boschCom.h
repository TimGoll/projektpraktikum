#ifndef boschCom_h
#define boschCom_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace communication {
    class BoschCom : public Thread {
    public:
        //Defaultconstructor
        BoschCom();
        //Destructor
        ~BoschCom();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
    };
}

#endif
