#ifndef MAIN_STORED_H
#define MAIN_STORED_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <SdFat.h>



namespace storage {
    class Main_StoreD : public Thread {
    public:
        //Defaultconstructor
        Main_StoreD();
        //Destructor
        ~Main_StoreD();
        bool button;
        //bool button_before;
        std::string filename;
        int filenumber;
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
    };
}

#endif
