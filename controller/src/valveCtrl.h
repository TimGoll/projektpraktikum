#ifndef VALVECTRL_H
#define VALVECTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h>

#include "ownlibs/serialCommunication.h"
#include "eventElement.h"
#include "main_display.h"

namespace control {
    class ValveCtrl {
    public:
        //Defaultconstructor
        ValveCtrl(int id);
        //Destructor
        ~ValveCtrl();
        //setzt den Pin des Ventils
        void setPin(int pin);
        //Stellwerte fuer die Ventile koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(int value, unsigned long time);
        //sobald diese Funktion ausgefuehrt wird, beginnt das Programm mit der Ansteuerung
        void start(unsigned long startTime);
        //Gebe Adresse des Displayobjektes an dieses Ventil, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Die compute()-Function wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool compute();
    private:
        int id;
        int pin;
        QueueList <eventElement> eventList;
        bool ready;
        unsigned long startTime;

        eventElement nextEvent;

        io::Main_Display *main_display;
    };
}

#endif
