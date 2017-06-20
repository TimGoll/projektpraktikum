#ifndef MAIN_LABCOM_H
#define MAIN_LABCOM_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "ownlibs/common.h"

#include "ownlibs/serialCommunication.h"
#include "config.h"
#include "main_mfcCtrl.h"
#include "main_valveCtrl.h"
#include "main_display.h"
#include "main_boschCom.h"
#include "main_stringBuilder.h"
#include "parseInput.h"

namespace communication {
    // an die MFCs werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class Main_LabCom : public Thread {
    public:
        //Defaultconstructor
        Main_LabCom();
        //Destructor
        ~Main_LabCom();
        //Gebe Adresse des Hauptobjektes (MFC) an LabCom, um zu kommunizieren
        void setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl);
        //Gebe Adresse des Hauptobjektes (Valve) an LabCom, um zu kommunizieren
        void setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl);
        //Gebe Adresse des Hauptobjektes (Bosch) an LabCom, um zu kommunizieren
        void setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom);
        //Gebe Adresse des Hauptobjektes (Display) an LabCom, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Gebe Adresse des Stringbuilders an LabCom
        void setMainStringBuilderObjectPointer(communication::Main_StringBuilder *main_stringBuilder);

        void setParseInputObjectPointer(communication::ParseInput *parseInput);
        //setze neue Zeile zur Uebertragung an LabView
        void setNewLine(char newLine[]);
        //gibt Befehl zum Start der Threads von MFC und Ventil, uebergibt die Startzeit, welche als
        //Nullpunkt dient
        void start();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        //Diese Funktion wird zu Programmstart mehrmals ausgefuehrt, bis die kompletten Daten
        //eingetroffen sind. Sie liefert bei erfolgreicher Ausfuehrung 1, ansonsten
        //einen Errorcode mit einer Fehlermeldung in der Konsole
        uint16_t readLine();

        //Adressen der Ventil, MFC und Display Hauptobjekte zur Verteilung der Daten
        control::Main_MfcCtrl *main_mfcCtrl;
        control::Main_ValveCtrl *main_valveCtrl;
        communication::Main_BoschCom *main_boschCom;
        io::Main_Display *main_display;
        communication::Main_StringBuilder *main_stringBuilder;
        communication::ParseInput *parseInput;

        bool reading;
        bool sending;

        char inDataBuffer[SERIAL_READ_MAX_LINE_SIZE];
        uint16_t bufferCharIndex;

        char inDataArray [SERIAL_READ_MAX_BLOCK_AMOUNT][SERIAL_READ_MAX_BLOCK_SIZE];
        uint16_t headerLineCounter;

        //Header-Varablen:
        uint16_t amount_MFC;
        uint16_t amount_valve;

        bool finished;
        bool mfc_finished;
        bool valve_finished;
    };
}

#endif
