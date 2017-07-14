#include "mfcCom_mks.h"

namespace communication {
    namespace Mks {
        //ohne Objektorientierung muessen Variablen im CPP deklariert werden
        IntervalTimer *eventTimer;
        bool busy;
        uint32_t transmission_startTime;

        float *destination_value_ptr;
        //ende

        void init() {
            eventTimer = new IntervalTimer();
            busy = false;
            transmission_startTime = 0;
        }

        bool writeValue(char address[], float value, float *destination) {
            //Wenn Antwort von MFC noch nicht erhalten wurde, dann gebe false zurueck,
            //Befehl muss erneut gesetzt werden
            if (busy && millis() - transmission_startTime < MKS_ANSWER_TIMEOUT)
                return false;
            busy = true;
            transmission_startTime = millis();
            destination_value_ptr = destination;

            digitalWrite(PIN_ENABLE_MKS, HIGH);

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

            //berechne Uebertragungszeit (microseconds)
            //Zeit pro Bit         : 1.0 / SERIAL_MKS_BAUDRATE * 1000000
            //Anzahl Bits pro Byte : 10 (start + stop bit)
            //Anzahl Bytes         : 12
            uint32_t transmission_time = 1.0 / (double) SERIAL_MKS_BAUDRATE * 1000000 * 10 * 12;
            transmission_time += 0.5 * MKS_READ_START_TIME;
            eventTimer->begin(_writeValue_changeEnable, transmission_time);

            srl->print('M', "@");
            srl->print('M', address);
            srl->print('M', "S"); //Setze Fluss
            srl->print('M', value_string);
            srl->print('M', '\r'); //Termimierung

            return true;
        }

        void _writeValue_changeEnable() {
            eventTimer->end(); //Timer soll nur einmal ausgefuehrt werden
            digitalWrite(PIN_ENABLE_MKS, LOW); //schalte in Lesemodus

            //berechne Uebertragungszeit (microseconds)
            //Zeit pro Bit         : 1.0 / SERIAL_MKS_BAUDRATE * 1000000
            //Anzahl Bits pro Byte : 10 (start + stop bit)
            //Anzahl Bytes         : 12 //TODO Antwortgroesse
            uint32_t transmission_time = 1.0 / (double) SERIAL_MKS_BAUDRATE * 1000000 * 10 * 12;
            transmission_time += 0.5 * MKS_READ_START_TIME;
            eventTimer->begin(_writeValue_readAnswer, transmission_time);
        }

        void _writeValue_readAnswer() {
            //Lesen nachdem Serielle Uebertragung der Antwort abgeschlossen ist
            eventTimer->end(); //Timer soll nur einmal ausgefuehrt werden
            busy = false; //COM Modul ist nach Abarbeitung dieses Auftrags wieder fuer neue Befehle bereit

            uint8_t charId = 0;
            uint32_t read_startTime = 0;
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

            //TODO: verarbeite Antwort
            srl->println('L', response);

            *destination_value_ptr = atoi(response); //TODO anpassen
        }

        bool readValue(char address[], float *destination) { //TODO wie write
            srl->print('M', "@");
            srl->print('M', address);
            srl->print('M', "F"); //Lese Fluss
            srl->print('M', '\r'); //Termimierung

            //TODO verarbeite Antwort
            return 0;
        }

    }
}
