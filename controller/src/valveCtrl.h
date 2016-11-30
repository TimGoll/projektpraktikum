#ifndef VALVECTRL_H
#define VALVECTRL_H

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
        //Die compute()-Function wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool compute();
    private:
    };
}

#endif
