#include "mfcCom_buerkert.h"

namespace communication {
    namespace Buerkert {
        //ohne Objektorientierung muessen Variablen im CPP deklariert werden
        IntervalTimer *eventTimer;
        bool busy;
        uint32_t transmission_startTime;

        float *destination_value_ptr;
        uint8_t estimated_answer_length;
        //ende

        void init() {
            eventTimer = new IntervalTimer();
            busy = false;
            transmission_startTime = 0;
            estimated_answer_length = 0;

            pinMode(PIN_ENABLE_BUERKERT, OUTPUT);
        }

        bool writeValue(char address[], float value, float *destination) {
            if (_writeCommand(_byte_adress(atoi(address)), 0x92, _byte4_floatToInt(value))) {
                destination_value_ptr = destination;
                estimated_answer_length = 8; //TODO
                return true;
            }
            return false;
        }

        bool readValue(char address[], float *destination) {
            if (_writeCommand(_byte_adress(atoi(address)), 0x03, 0)) {
                destination_value_ptr = destination;
                estimated_answer_length = 8; //TODO
                return true;
            }
            return false;
        }


        //HELPERFUNKTIONEN
        uint8_t _byte_adress(uint8_t address) {
            uint8_t parameter = 0b10000000;

            //setze ersten beiden bits auf _10_
            uint8_t return_value = parameter | address;

            return return_value;
        }

        uint8_t _byte_checksum(uint8_t start_sign, uint8_t address, uint8_t command, uint16_t answer, uint32_t data) {
            //convert to 8-Bit
            union {
                uint8_t _16_to_8_bit_array[2];
                uint16_t _16_to_8_bit;
            };
            union {
                uint8_t _32_to_8_bit_array[4];
                uint32_t _32_to_8_bit;
            };

            _16_to_8_bit = answer;
            _32_to_8_bit = data;

            uint8_t byte_checksum = 0b00000000; //0

            byte_checksum = byte_checksum ^ start_sign;
            byte_checksum = byte_checksum ^ address;
            byte_checksum = byte_checksum ^ command;
            byte_checksum = byte_checksum ^ _16_to_8_bit_array[0]; //answer
            byte_checksum = byte_checksum ^ _16_to_8_bit_array[1];
            byte_checksum = byte_checksum ^ _32_to_8_bit_array[0]; //data
            byte_checksum = byte_checksum ^ _32_to_8_bit_array[1];
            byte_checksum = byte_checksum ^ _32_to_8_bit_array[2];
            byte_checksum = byte_checksum ^ _32_to_8_bit_array[3];

            return byte_checksum;
        }

        uint32_t _byte4_floatToInt(float value) {
            //Float zu int "Converter", kopiert bits, aendert aber Datentyp
            typedef union
            {
                float    _value_f;
                uint32_t _value_i;
            } FloatBits;

            FloatBits fb;
            fb._value_f = value;

            return fb._value_i;
        }


    

        bool _writeCommand(uint8_t address, uint8_t command, uint32_t value) { //value = data
            //Wenn Antwort von MFC noch nicht erhalten wurde, dann gebe false zurueck,
            //Befehl muss erneut gesetzt werden (vorheriger Befehl)
            if (busy && millis() - transmission_startTime < MKS_ANSWER_TIMEOUT)
                return false;
            busy = true;
            transmission_startTime = millis();

            digitalWrite(PIN_ENABLE_BUERKERT, HIGH);

            //Befehlslaenge
            uint8_t command_length = 13; //TODO sollte unterschiedliche Laenge haben

            //berechne Uebertragungszeit (microseconds)
            //Zeit pro Bit         : 1.0 / SERIAL_BUERKERT_BAUDRATE * 1000000
            //Anzahl Bits pro Byte : 10 (start + stop bit)
            //Anzahl Bytes         : command_length
            uint32_t transmission_time = 1.0 / (double) SERIAL_BUERKERT_BAUDRATE * 1000000 * 10 * command_length;
            transmission_time += 0.5 * BUERKERT_READ_START_TIME; //Wechsel Modus in Mitte der Uebertragungspause
            eventTimer->begin(_writeCommand_changeEnable, transmission_time);

            srl->print('B', 0xFF); //preambel
            srl->print('B', 0xFF); //preambel
            srl->print('B', 0xFF); //preambel
            srl->print('B', 0x02); //startsign (master->slave)
            srl->print('B', address);
            srl->print('B', command); //TODO 4byte 0 wenn kein read?
            srl->print('B', 0x05); //5 byte folgen (nur daten, keine Antwort)
            srl->print('B', 0x01); //Externer Soll-Wert
            srl->print('B', value);
            srl->print('B', _byte_checksum(
                                0x02, //startsign
                                address,
                                command,
                                0x00, //answer
                                value
                            ));

            return true;
        }

        void _writeCommand_changeEnable() {
            eventTimer->end(); //Timer soll nur einmal ausgefuehrt werden
            digitalWrite(PIN_ENABLE_MKS, LOW); //schalte in Lesemodus

            //berechne Uebertragungszeit (microseconds)
            //Zeit pro Bit         : 1.0 / SERIAL_MKS_BAUDRATE * 1000000
            //Anzahl Bits pro Byte : 10 (start + stop bit)
            //Anzahl Bytes         : estimated_answer_length
            uint32_t transmission_time = 1.0 / (double) SERIAL_MKS_BAUDRATE * 1000000 * 10 * estimated_answer_length;
            transmission_time += 0.5 * MKS_READ_START_TIME;
            eventTimer->begin(_writeCommand_readAnswer, transmission_time);
        }

        void _writeCommand_readAnswer() { //TODO Antwort muss verarbeitet
            //Lesen nachdem Serielle Uebertragung der Antwort abgeschlossen ist
            eventTimer->end(); //Timer soll nur einmal ausgefuehrt werden
            busy = false; //COM Modul ist nach Abarbeitung dieses Auftrags wieder fuer neue Befehle bereit

            //*destination_value_ptr = atoi(response); //TODO anpassen
        }
    }
}
