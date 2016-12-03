#ifndef MAIN_VALVECTRL_H
#define MAIN_VALVECTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "valveCtrl.h"

namespace control {
    // an die Ventile werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class Main_ValveCtrl : public Thread {
    public:
        //Defaultconstructor
        Main_ValveCtrl();
        //Destructor
        ~Main_ValveCtrl();
        //Funktion, die von LabCom aufgerufen wird. Sie erstellt Valve-Objekte
        void createValve(int amount);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Pins.
        //Adressen werden weiter an alle Valve-Objekte gegeben
        void setPins(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Stellwerte fuer die MFCs koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(int valveID, int value, unsigned long time);
        //setzt die 'ready'-Variable der Valves auf true. Außerdem wird der Nullpunkt der Steuerung gesetzt
        void start(unsigned long startTime);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int amount_valve;
        control::ValveCtrl *valve_list[MAX_AMOUNT_VALVE]; //Hier werden die Adressen der Valve-Objekte gespeichert
        bool valve_continue_next_loop[MAX_AMOUNT_VALVE];
    };
}

#endif
