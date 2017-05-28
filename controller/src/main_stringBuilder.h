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

#include "ownlibs/common.h"
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
        void setIntervall(uint16_t intervall);
        //aktiviere Klasse von LabCom aus, setzt die erste Intervallzeit
        void start(uint32_t time);
        //LabCom meldet, dass beide Queues abgearbeitet sind, Meldung wird auf dem Display
        //angezeigt
        void bothQueuesFinished ();
        //speichere Datumstring
        void setDateString(char dateString[]);

        //Uebergebe Objektpointer
        void setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl);
        void setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl);
        void setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        bool ready;
        uint32_t lastTime;
        uint32_t startTime;
        uint16_t intervall;
        char dateString[32];

        uint16_t mfcValueList[MAX_AMOUNT_MFC];
        uint16_t valveValueList[MAX_AMOUNT_VALVE];

        char currentValveValue[16];
        char currentMfcValue[16];
        char currentBoschValue[16];

        char newLine[SERIAL_READ_MAX_LINE_SIZE];

        storage::StoreD *storeD; //Hier wird das StoreD-Objekt gespeichert
        control::Main_ValveCtrl *main_valveCtrl;
        control::Main_MfcCtrl *main_mfcCtrl;
        communication::Main_BoschCom *main_boschCom;
    };
}

#endif
