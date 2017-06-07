#ifndef PARSEINPUT_H
#define PARSEINPUT_H

#include "config.h"

#include "main_mfcCtrl.h"
#include "main_valveCtrl.h"
#include "main_display.h"
#include "main_boschCom.h"

#include "ownlibs/common.h"
#include "ownlibs/serialCommunication.h"

namespace communication {
    class ParseInput {
    public:
        //Defaultconstructor
        ParseInput();
        //Destructor
        ~ParseInput();
        //setze neue Zeile. Kann von LabCom oder SD(StringBuilder) kommen
        //gibt 0 oder Errorcode zurueck
        uint16_t parseNewLine(char newLine[]);

        //Gebe Adresse des Hauptobjektes (MFC) an ParseInput, um zu kommunizieren
        void setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl);
        //Gebe Adresse des Hauptobjektes (Valve) an ParseInput, um zu kommunizieren
        void setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl);
        //Gebe Adresse des Hauptobjektes (Bosch) an ParseInput, um zu kommunizieren
        void setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom);
        //Gebe Adresse des Hauptobjektes (Display) an ParseInput, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
    private:
        control::Main_MfcCtrl *main_mfcCtrl;
        control::Main_ValveCtrl *main_valveCtrl;
        communication::Main_BoschCom *main_boschCom;
        io::Main_Display *main_display;

        uint8_t _headerLineCounter;
    };
}

#endif
