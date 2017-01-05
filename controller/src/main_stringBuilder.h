#ifndef MAIN_BOSCHCOM_H
#define MAIN_BOSCHCOM_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

namespace communication {
    class Main_StringBuilder : public Thread {
    public:
        //Defaultconstructor
        Main_StringBuilder();
        //Destructor
        ~Main_StringBuilder();
        //setze das Intervall, in welchem die Hauptschleife ausgefuert wird.
        //Zeit ist gleich der des Sensors
        void Main_StringBuilder::setIntervall(int intervall);
        //aktiviere Klasse von LabCom aus, setzt die erste Intervallzeit
        void Main_StringBuilder::start(unsigned long time);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        bool ready;
        unsigned long lastTime;
        int intervall;
    };
}

#endif
