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
        bool writeValue(char type[], char address[], float value, float *destination);
        //lese Wert eines MFCs
        bool readValue(char type[], char address[], float *destination);
    private:
    };

    extern MfcCom *mfcCom;
}


#endif
