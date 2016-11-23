#ifndef mfcCtrl_h
#define mfcCtrl_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace control {
    class MfcCtrl : public Thread {
    public:
        //Defaultconstructor
        MfcCtrl();
        //Destructor
        ~MfcCtrl();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
    };
}

#endif
