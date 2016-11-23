#ifndef labCom_h
#define labCom_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace communication {
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
    };
}

#endif
