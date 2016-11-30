#ifndef MAIN_VALVECTRL_H
#define MAIN_VALVECTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "mfcCtrl.h"

namespace control {
    // an die MFCs werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class Main_ValveCtrl : public Thread {
    public:
        //Defaultconstructor
        Main_ValveCtrl();
        //Destructor
        ~Main_ValveCtrl();
        //Funktion, die von LabCom aufgerufen wird. Sie erstellt MFC-Objekte
        void createValve(int amount);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Adressen.
        //Adressen werden weiter an alle MFC-Objekte gegeben
        void setPins(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Typen
        //Typen werden weiter an alle MFC-Objekte gegeben.
        void setEvent(int valveID, int value, unsigned long time);
        //setzt die 'ready'-Variable der MFCs auf true. Außerdem wird der Nullpunkt der Steuerung gesetzt
        void start(unsigned long startTime);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int amount_valve;
        //control::MfcCtrl *mfc_list[MAX_AMOUNT_MFC]; //Hier werden die Adressen der MFC-Objekte gespeichert
        //bool mfc_continue_next_loop[MAX_AMOUNT_MFC];
    };
}

#endif
