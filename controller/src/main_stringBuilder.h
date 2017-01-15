#ifndef MAIN_STRINGBUILDER_H
#define MAIN_STRINGBUILDER_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "StoreD.h" //StoreD wird vom StringBuilder verwaltet und aufgerufen

#include "main_valveCtrl.h" //Objekte zum Speichern der Objektpointer
#include "main_mfcCtrl.h"
#include "main_boschCom.h"

#include "config.h"

#include "ownlibs/serialCommunication.h"

namespace communication {
    class Main_StringBuilder : public Thread {
    public:
        //Defaultconstructor
        Main_StringBuilder();
        //Destructor
        ~Main_StringBuilder();
        //setze das Intervall, in welchem die Hauptschleife ausgefuert wird.
        //Zeit ist gleich der des Sensors
        void setIntervall(int intervall);
        //aktiviere Klasse von LabCom aus, setzt die erste Intervallzeit
        void start(unsigned long time);

        //Uebergebe Objektpointer
        void setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl);
        void setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl);
        void setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        bool ready;
        unsigned long lastTime;
        int intervall;
        char newLine[SERIAL_READ_MAX_LINE_SIZE];
        int mfcValueList[];
        int valveValueList[];
        char currentValveValue[16];
        char currentMfcValue[16];
        char currentBoschValue[16];


        storage::StoreD *storeD; //Hier wird das StoreD-Objekt gespeichert
        control::Main_ValveCtrl *main_valveCtrl;
        control::Main_MfcCtrl *main_mfcCtrl;
        communication::Main_BoschCom *main_boschCom;
    };
}

#endif
