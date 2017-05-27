#ifndef MAIN_BOSCHCOM_H
#define MAIN_BOSCHCOM_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include <bme280.h>

#include "ownlibs/serialCommunication.h"

namespace communication {
    class Main_BoschCom : public Thread {
    public:
        //Defaultconstructor
        Main_BoschCom();
        //Destructor
        ~Main_BoschCom();
        //setzt Messintervall
        void setIntervall(int intervall);
        //aktiviert den Sensor, startet Messung
        void start(unsigned long time);
        //gibt aktuellen Messwert des Sensors zurueck
        int getCurrentValue();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int intervall;
        bool ready;
        unsigned long lastTime;

        int currentValue;

        Adafruit_BME280 *bme280;
    };
}

#endif
