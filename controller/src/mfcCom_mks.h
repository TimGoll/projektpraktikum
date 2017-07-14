#ifndef MFCCOM_MKS_H
#define MFCCOM_MKS_H

#include <Arduino.h>

#include "config.h"
#include "ownlibs/serialCommunication.h"

namespace communication {
    namespace Mks {
        //"public" functions
        void init();

        //setzt einen Schreibauftrag an einen MFC auf. Mittels Interrupts wird die
        //zeitliche Steuerung garantiert. Der destination Pointer dient dazu um
        //asynchron den Wert des MFCs zurueck zu geben.
        //Die Funktion gibt false zurueck, wenn sie gerade noch beschaeftig ist,
        //der Befehl muss dann erneut gesendet werden.
        bool writeValue(char address[], float value, float *destination);
        bool readValue(char address[], float *destination);

        //"private" functions
        void _writeValue_changeEnable();
        void _writeValue_readAnswer();
    }
}

#endif
