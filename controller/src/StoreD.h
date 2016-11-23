#ifndef StoreD_h
#define StoreD_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace storage {
    class StoreD : public Thread {
    public:
        //Defaultconstructor
        StoreD();
        //Destructor
        ~StoreD();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
    };
}

#endif
