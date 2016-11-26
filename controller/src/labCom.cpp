#include "labCom.h"

namespace communication {
    LabCom::LabCom() {
        this->reading = true;
        this->sending = false;
        this->bufferCharIndex = 0;
    }
    LabCom::~LabCom() {

    }

    bool LabCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        // TODO: Eventuell immer Zeilenweise lesen mit while(Serial.available()), ist zu testen.
        // Verlagerung des Codes in seperate Lese/Schreibfunktion sinnvoll
        if (this->reading) { //Empfange Messprogramm
            if (Serial.available() > 0) { //Lese Daten, wenn vorhanden
                this->singleCharBuffer = Serial.read(); //Serial.read() gibt einen einzelnen Char zurueck

                //UEBERPRUEFUNG
                if (this->bufferCharIndex == 0) {
                    if (singleCharBuffer != '<') { //erstes Zeichen muss oeffnender Tag sein
                        Serial.println("ERROR - Falscher Zeilenbeginn");
                        return false; //Stoppe thread und warte auf Neustart
                    }
                }
                if (singleCharBuffer == '\n') { //Zeile zuende
                    this->bufferCharIndex = -1; //Workaround: wird am Ende nochmal um 1 erhoet
                    if (this->inDataBuffer[this->bufferCharIndex -1] != '>') { //vorheriges Zeichen muss schliessender Tag sein
                        Serial.println("ERROR - Falsches Zeilenende");
                        return false; //Stoppe thread und warte auf Neustart
                    }
                }

                this->inDataBuffer[this->bufferCharIndex] = this->singleCharBuffer;

                // Wenn this->bufferCharIndex = -1 ist, dann ist der String abgeschlossen und vollstauendig
                // und kann zerlegt werden und an MFCs + Ventile uergeben werden

                this->bufferCharIndex++;
            }
        }

        if (this->sending) { //Sende Messwerte parallel zur Messung

        }

        //Serial.println("loop called");
        return true;
    }
}
