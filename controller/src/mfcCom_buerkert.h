#ifndef MFCCOM_BUERKERT_H
#define MFCCOM_BUERKERT_H

#include <Arduino.h>

#include "ownlibs/serialCommunication.h"

namespace communication {
    class MfcCom_Buerkert {
    public:
        //Defaultconstructor
        MfcCom_Buerkert();
        //Destructor
        ~MfcCom_Buerkert();
        //schreibe Soll-Wert an MFC, gebe Fehlercode zurueck, falls voranden
        uint8_t writeValue(char address[], float value);   // ExtSetpoint
        //lese Wert eines MFCs
        uint32_t readValue(char address[]);                // ReadCurrentAndFourDynamicVariables
        //lese maximalwert
        //uint32_t readMaxFlow(uint8_t address);
    private:
        //bekomme 6-bit Adresse (0..32) und setze Bit 7+8
        uint8_t byte_adress(uint8_t address);
        //Checksumme generieren
        uint8_t byte_checksum(uint8_t start_sign, uint8_t address, uint8_t command, uint16_t answer, uint32_t data);

        uint32_t byte4_floatToInt(float value);

        //temporaere Variablen fuer die Uebertragung
        uint8_t _preambel;
        uint8_t _startsign;
        uint8_t _address;
        uint8_t _command;
        uint8_t _checksum;
        uint16_t _answer;
        uint32_t _data;

        //temporare Variablen, um alles auf 8-bit zu setzen
        union {
            struct {
                uint8_t _16_to_8_bit_0;
                uint8_t _16_to_8_bit_1;
            };
            uint16_t _16_to_8_bit;
        };
        union {
            struct {
                uint8_t _32_to_8_bit_0;
                uint8_t _32_to_8_bit_1;
                uint8_t _32_to_8_bit_2;
                uint8_t _32_to_8_bit_3;
            };
            uint16_t _32_to_8_bit;
        };

        //float als integer casten
        typedef union
        {
            float    _value_f;
            uint32_t _value_i;
        } FloatBits;
    };
}

#endif
