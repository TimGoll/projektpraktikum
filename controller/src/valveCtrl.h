#ifndef VALVECTRL_H
#define VALVECTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h>

#include "ownlibs/serialCommunication.h"
#include "ownlibs/pca9555.h"
#include "eventElement.h"
#include "main_display.h"

namespace control {
    class ValveCtrl {
    public:
        //Defaultconstructor
        ValveCtrl(uint16_t id);
        //Destructor
        ~ValveCtrl();
        //setze Pointer des Com-Objects
        void setComPointer(Pca9555 *pca9555);
        //setzt den Pin des Ventils
        void setPin(uint16_t pin);
        //Stellwerte fuer die Ventile koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(uint16_t value, uint32_t time);
        //sobald diese Funktion ausgefuehrt wird, beginnt das Programm mit der Ansteuerung
        void start(uint32_t startTime);
        //Gebe Adresse des Displayobjektes an dieses Ventil, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Gibt den aktuellen Soll-Wert des Ventils zurueck
        uint8_t getCurrentValue();
        //Die compute()-Function wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool compute();
    private:
        uint16_t id;
        uint16_t pin;
        bool     ready;
        uint32_t startTime;
        int8_t   currentValue;

        QueueList <eventElement_valve> eventList;
        eventElement_valve nextEvent;

        io::Main_Display *main_display;
        Pca9555 *pca9555;
    };
}

#endif
