#ifndef MFCCOM_BUERKERT_H
#define MFCCOM_BUERKERT_H

#include <Arduino.h>

#include "config.h"
#include "ownlibs/serialCommunication.h"

namespace communication {
    namespace Buerkert {
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
        uint8_t _byte_adress(uint8_t address);
        uint8_t _byte_checksum(uint8_t start_sign, uint8_t address, uint8_t command, uint16_t answer, uint32_t data);
        uint32_t _byte4_floatToInt(float value);

        bool _writeCommand(uint8_t address, uint8_t command, uint32_t value_string);
        void _writeCommand_changeEnable();
        void _writeCommand_readAnswer();
    }
}

#endif
