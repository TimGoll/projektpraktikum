#ifndef MFCCOM_H
#define MFCCOM_H

#include <Arduino.h>

#include "mfcCom_buerkert.h"
#include "mfcCom_mks.h"

namespace communication {
    class MfcCom {
    public:
        //Defaultconstructor
        MfcCom();
        //Destructor
        ~MfcCom();
        //schreibe Soll-Wert an MFC, gebe Fehlercode zurueck, falls voranden
        uint8_t writeValue(char type[], char address[], float value);
        //lese Wert eines MFCs
        uint32_t readValue(char type[], char address[]);
    private:
        MfcCom_Mks *_mks;
        MfcCom_Buerkert *_buerkert;
    };

    extern MfcCom *mfcCom;
}


#endif
