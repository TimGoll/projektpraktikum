#include "mfcCom_mks.h"

namespace communication {
    namespace Mks {
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

            pinMode(PIN_ENABLE_MKS, OUTPUT);
            digitalWrite(PIN_ENABLE_MKS, HIGH);
        }

        bool writeValue(char address[], float value, float *destination) {
            //calculate value string
            char value_string[8];
            uint8_t first_digit = (uint8_t) value;
            uint32_t last_digits = 0;
            if (first_digit < 10) {
                last_digits = (value - first_digit) * 100000;
                sprintf(value_string, "%d.%05lu", first_digit, last_digits);
            } else {
                last_digits = (value - first_digit) * 10000;
                sprintf(value_string, "%d.%04lu", first_digit, last_digits);
            }

            if (_writeCommand(address, 'S', value_string)) {
                destination_value_ptr = destination;
                estimated_answer_length = 8; //TODO
                return true;
            }
            return false;
        }

        bool readValue(char address[], float *destination) {
            if (_writeCommand(address, 'F', (char*) "")) {
                destination_value_ptr = destination;
                estimated_answer_length = 8; //TODO
                return true;
            }
            return false;
        }


        //HELPERFUNKTIONEN

        bool _writeCommand(char address[], char command, char value_string[]) {
            //Wenn Antwort von MFC noch nicht erhalten wurde, dann gebe false zurueck,
            //Befehl muss erneut gesetzt werden (vorheriger Befehl)
            if (busy && millis() - transmission_startTime < MKS_ANSWER_TIMEOUT)
                return false;
            busy = true;
            transmission_startTime = millis();

            digitalWrite(PIN_ENABLE_MKS, HIGH);

            //Berechne Befehlslaenge
            uint8_t command_length = 5; //preambel, address, commandChar, termination
            for (uint8_t i = 0; value_string[i] != '\0'; i++)
                command_length++;

            //berechne Uebertragungszeit (microseconds)
            //Zeit pro Bit         : 1.0 / SERIAL_MKS_BAUDRATE * 1000000
            //Anzahl Bits pro Byte : 10 (start + stop bit)
            //Anzahl Bytes         : command_length
            uint32_t transmission_time = 1.0 / (double) SERIAL_MKS_BAUDRATE * 1000000 * 10 * command_length;
            transmission_time += 0.5 * MKS_READ_START_TIME; //Wechsel Modus in Mitte der Uebertragungspause
            eventTimer->begin(_writeCommand_changeEnable, transmission_time);

            srl->print('M', "@");
            srl->print('M', address);
            srl->print('M', command); //Setze Fluss
            srl->print('M', value_string);
            srl->print('M', '\r'); //Termimierung

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

        void _writeCommand_readAnswer() {
            //Lesen nachdem Serielle Uebertragung der Antwort abgeschlossen ist
            eventTimer->end(); //Timer soll nur einmal ausgefuehrt werden
            busy = false; //COM Modul ist nach Abarbeitung dieses Auftrags wieder fuer neue Befehle bereit

            uint8_t charId = 0;
            uint32_t read_startTime = millis();
            char response[16];
            while (millis() - read_startTime < MKS_READ_TIMEOUT) {
                 //warte, wenn gerade kein Zeichen in Buffer, aber String nicht abgeschlossen
                if (srl->available('M') <= 0)
                    continue;
                char newChar = srl->read('M');
                if (newChar == '\r') { //string abgeschlossen
                    response[charId] = '\0';
                    break;
                }
                response[charId] = newChar;
                charId++;
            }
            digitalWrite(PIN_ENABLE_MKS, HIGH); //setze Pin wieder HIGH, damit writeEnable aktiviert ist

            if (response[0] == '@' && response[1] == '-') {
                char response_new[16];
                uint8_t i = 4;
                for (i; response[i] != '\0'; i++) {
                    response_new[i-4] = response[i];
                }
                response_new[i-4] = '\0';

                *destination_value_ptr = cmn::charArrayToFloat(response_new);
            } else {
                *destination_value_ptr = 0.0;
                srl->println('D', "MFC Antwortet inkorrekt.");
            }
        }
    }
}
