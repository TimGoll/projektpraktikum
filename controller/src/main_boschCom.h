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
        void setIntervall(uint16_t intervall);
        //aktiviert den Sensor, startet Messung
        void start(uint32_t time);
        //gibt aktuellen Messwert des Sensors zurueck (Pointer auf Array)
        float* getCurrentValues();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        uint16_t intervall;
        bool ready;
        uint32_t lastTime;

        //[Temeoeratur, Luftdruck, Luftfeuchtigkeit]
        float currentValues[3];

        Adafruit_BME280 *bme280;
    };
}

#endif
