#ifndef MFCCOM_MKS_H
#define MFCCOM_MKS_H

#include <Arduino.h>

#include "ownlibs/serialCommunication.h"

namespace communication {
    class MfCCom_Mks {
    public:
        //Defaultconstructor
        MfCCom_Mks();
        //Destructor
        ~MfCCom_Mks();
        //schreibe Soll-Wert an MFC, gebe Fehlercode zurueck, falls voranden
        uint8_t writeValue(uint8_t address, float value);
        //lese Wert eines MFCs
        uint32_t readValue(uint8_t address);
    private:
        char _address[3];
        char _value[8];
    };
}

#endif
