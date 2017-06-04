#ifndef MFCCOM_MKS_H
#define MFCCOM_MKS_H

#include <Arduino.h>

#include "ownlibs/serialCommunication.h"

namespace communication {
    class MfcCom_Mks {
    public:
        //Defaultconstructor
        MfcCom_Mks();
        //Destructor
        ~MfcCom_Mks();
        //schreibe Soll-Wert an MFC, gebe Fehlercode zurueck, falls voranden
        uint8_t writeValue(char address[], float value);
        //lese Wert eines MFCs
        uint32_t readValue(char address[]);
    private:
        char _address[3];
        char _value[8];
    };
}

#endif
