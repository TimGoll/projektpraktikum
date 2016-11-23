#ifndef ValveCtrl_h
#define ValveCtrl_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace control {
    class ValveCtrl : public Thread {
    public:
        //Defaultconstructor
        ValveCtrl();
        //Destructor
        ~ValveCtrl();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
    };
}

#endif
