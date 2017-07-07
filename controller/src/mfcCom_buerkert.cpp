#include "mfcCom_buerkert.h"

namespace communication {
    MfcCom_Buerkert::MfcCom_Buerkert() {

    }

    MfcCom_Buerkert::~MfcCom_Buerkert() {

    }

    uint8_t MfcCom_Buerkert::writeValue(char address[], float value) {
        this->_preambel  = 0xFF; //sende 2 bis 20 Mal zur Initilisierung
        this->_startsign = 0x02; //Master->Slave
        this->_address   = byte_adress(atoi(address));
        this->_command   = 0x92; //Setze Wert
        this->_answer    = 0; //keine Antwort hier, dennoch 2 Byte gross
        this->_data      = this->byte4_floatToInt(value);
        this->_checksum  = this->byte_checksum(
                               this->_startsign,
                               this->_address,
                               this->_command,
                               this->_answer,
                               this->_data
                           );

        //Uebertrage Daten
        digitalWrite(PIN_ENABLE_BRK, HIGH); //TODO kurzes Dalay hiernach?
        srl->print('B', this->_preambel);
        srl->print('B', this->_preambel);
        srl->print('B', this->_preambel);
        srl->print('B', this->_startsign);
        srl->print('B', this->_address);
        srl->print('B', this->_command);
        srl->print('B', 0x05); //5 byte folgen (nur daten, keine Antwort)
        srl->print('B', 0x01); //Externer Soll-Wert
        srl->print('B', this->_data);
        srl->print('B', this->_checksum);
        digitalWrite(PIN_ENABLE_BRK, LOW);

        srl->println('B', ""); //DEBUG: zeilenumbruch

        return 0; //TODO
    }

    uint32_t MfcCom_Buerkert::readValue(char address[]) {
        //TODO lese Wert
        return 0;
    }



    uint8_t MfcCom_Buerkert::byte_adress(uint8_t address) {
        uint8_t parameter = 0b10000000;

        //setze ersten beiden bits auf _10_
        uint8_t return_value = parameter | address;

        return return_value;
    }

    uint8_t MfcCom_Buerkert::byte_checksum(uint8_t start_sign, uint8_t address, uint8_t command, uint16_t answer, uint32_t data) {
        //convert to 8-Bit
        this->_16_to_8_bit = answer;
        this->_32_to_8_bit = data;

        uint8_t byte_checksum = 0b00000000; //0

        byte_checksum = byte_checksum ^ start_sign;
        byte_checksum = byte_checksum ^ address;
        byte_checksum = byte_checksum ^ command;
        byte_checksum = byte_checksum ^ this->_16_to_8_bit_0; //answer
        byte_checksum = byte_checksum ^ this->_16_to_8_bit_1;
        byte_checksum = byte_checksum ^ this->_32_to_8_bit_0; //data
        byte_checksum = byte_checksum ^ this->_32_to_8_bit_1;
        byte_checksum = byte_checksum ^ this->_32_to_8_bit_2;
        byte_checksum = byte_checksum ^ this->_32_to_8_bit_3;

        return byte_checksum;
    }

    uint32_t MfcCom_Buerkert::byte4_floatToInt(float value) {
        //Float zu int "Converter", kopiert bits, aendert aber Datentyp
        FloatBits fb;
        fb._value_f = value;

        return fb._value_i;
    }
}
