namespace communication {
    uint8_t MfcCom_Mks::writeValue(uint8_t address, float value) {
        this->_preambel  = 0xFF; //sende 2 bis 20 Mal zur Initilisierung
        this->_startsign = 0x02; //Master->Slave
        this->_address   = byte_adress(address);
        this->_command   = 0x92; //Setze Wert
        this->_answer    = 0; //keine Antwort hier, dennoch 2 Byte gross
        this->_data      = byte4_floatToInt(value);
        this->_checksum  = byte_checksum(
                               this->start_sign,
                               this->address,
                               this->command,
                               this->answer,
                               this->data
                           );

        //Uebertrage Daten
        digitalWrite(PIN_ENABLE_BRK, HIGH); //TODO kurzes Dalay hiernach?
        srl->print('U', this->_preambel);
        srl->print('U', this->_preambel);
        srl->print('U', this->_preambel);
        srl->print('U', this->_startsign)
        srl->print('U', this->_address);
        srl->print('U', this->_command);
        srl->print('U', 0x05); //5 byte folgen (nur daten, keine Antwort)
        srl->print('U', 0x01); //Externer Soll-Wert
        srl->print('U', this->_data);
        srl->print('U', this->_checksum);
        digitalWrite(PIN_ENABLE_BRK, LOW);
    }





    uint8_t MfcCom_Mks::byte_adress(uint8_t address) {
        uint8_t parameter = 0b10000000;

        //setze ersten beiden bits auf _10_
        return parameter | address;
    }

    uint8_t MfcCom_Mks::byte_checksum(uint8_t start_sign, uint8_t address, uint8_t command, uint16_t answer, uint32_t data) {
        //convert to 8-Bit
        this->_16_to_8_bit = answer;
        this->_32_to_8_bit = data;

        uint8_t byte_checksum = 0b00000000; //0

        byte_checksum = byte_checksum ^ start_sign;
        byte_checksum = byte_checksum ^ address;
        byte_checksum = byte_checksum ^ command;
        byte_checksum = byte_checksum ^ this->_16_to_8_bit_0; //answer
        byte_checksum = byte_checksum ^ this->_16_to_8_bit_1;
        byte_checksum = byte_checksum ^ this->_23_to_8_bit_0; //data
        byte_checksum = byte_checksum ^ this->_23_to_8_bit_1;
        byte_checksum = byte_checksum ^ this->_23_to_8_bit_2;
        byte_checksum = byte_checksum ^ this->_23_to_8_bit_3;

        return byte_checksum;
    }

    uint32_t MfcCom_Mks::byte4_floatToInt(float value) {
        //Float zu int "Converter", kopiert bits, aendert aber Datentyp
        FloatBits fb;
        fb._value_f = value;

        return fb._value_i;
    }
}
