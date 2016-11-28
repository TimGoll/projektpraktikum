#ifndef mfc_main_h
#define mfc_main_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "mfcCtrl.h"

namespace control {
    // an die MFCs werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class MfcMain : public Thread {
    public:
        //Defaultconstructor
        MfcMain();
        //Destructor
        ~MfcMain();
        //Funktion, die von LabCom aufgerufen wird. Sie erstellt MFC-Objekte
        void createMFC(int amount);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Adressen.
        //Adressen werden weiter an alle MFC-Objekte gegeben
        void setAdresses(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und enthält Eventdaten. Wichtig ist hier, dass
        //dieser Aufruf immer nur fuer EIN MFC ist, daher ist die ID von Noeten
        void setEvent(int mfcID, int value, unsigned int time);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int amount_MFC;
        control::MfcCtrl *mfc_list[MAX_AMOUNT_MFC]; //Hier werden die Adressen der MFC-Objekte gespeichert
    };
}

#endif
