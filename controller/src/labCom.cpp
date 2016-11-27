#include "labCom.h"

namespace communication {
    LabCom::LabCom() {
        this->reading = true;
        this->sending = false;

        this->headerLineCounter = 0;

        Serial.println("LabCom erstellt.");
    }
    LabCom::~LabCom() {

    }

    int LabCom::readLine() {
        this->bufferCharIndex = 0; //setze index auf Startposition zurueck
        this->startTime = millis();

        //Lese so lange, bis Zeile vollstaendig empfangen wurde, oder Timeout erreicht wurde (Main-Thread wird blockiert)
        while(millis() < this->startTime + SERIAL_READ_TIMEOUT) {
            if (Serial.available() > 0) { //Lese Daten, wenn vorhanden
                this->singleCharBuffer = Serial.read(); //Serial.read() gibt einen einzelnen Char zurueck

                //UEBERPRUEFUNG
                if (this->bufferCharIndex == 0) {
                    if (singleCharBuffer != '<') { //erstes Zeichen muss oeffnender Tag sein
                        Serial.println("ERROR - Falscher Zeilenbeginn");
                        return ERR_SERIAL_READ_WRONG_LINE_BEGIN;
                    }
                }
                if (singleCharBuffer == '\n') { //Zeile zuende
                    if (this->inDataBuffer[this->bufferCharIndex -1] != '>') { //vorheriges Zeichen muss schliessender Tag sein
                        Serial.println("ERROR - Falsches Zeilenende");
                        return ERR_SERIAL_READ_WRONG_LINE_ENDING;
                    }
                }

                this->inDataBuffer[this->bufferCharIndex] = this->singleCharBuffer;

                if (this->singleCharBuffer == '\n') { //vollstaendiger String abgeschlossen
                    this->inDataBuffer[this->bufferCharIndex +1] = '\0';
                    Serial.print("Eingabestring akzeptiert: ");
                    Serial.print(this->inDataBuffer);
                    return 1;
                }

                this->bufferCharIndex++;
                if (this->bufferCharIndex == SERIAL_READ_MAX_LINE_SIZE) {
                    return ERR_SERIAL_READ_MAX_STRING_SIZE;
                    Serial.println("ERROR - Eingabestring zu lang");
                }
            }
        }
        Serial.println("ERROR - Timeout");
        return ERR_SERIAL_READ_TIMEOUT; //Timeout
    }

    int LabCom::splitLine() {
        int currentCharIndex = 0;
        int endCharIndex = this->bufferCharIndex -1; //da letztes Zeichen '>' ist

        //Schleife, die durch die Ziel-Array-Indexe navigiert
        for (int arrayIndex = 0; arrayIndex < SERIAL_READ_MAX_BLOCK_AMOUNT; arrayIndex++) {
            //Schleife, die durch die Char-Positionen eines Ziel-Array-Eintrags navigiert
            for (int arrayCharIndex = 0; arrayCharIndex < SERIAL_READ_MAX_BLOCK_SIZE; arrayCharIndex++) {
                currentCharIndex++; //Wird anfangs um 1 erhoet, Zeichen 1 ist zu ignorieren ('<')

                if (this->inDataBuffer[currentCharIndex] == ',') { //Eintrag zu ende
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    break;
                }

                if (currentCharIndex == endCharIndex) { //Zeilenende erreicht
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    return arrayIndex +1; //Index und Groesse sind immer um 1 verschieden
                }

                this->inDataArray[arrayIndex][arrayCharIndex] = this->inDataBuffer[currentCharIndex];
            }
        }
    }

    //////////////////// MAINLOOP ////////////////////

    bool LabCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        // TODO: Eventuell immer Zeilenweise lesen mit while(Serial.available()), ist zu testen.
        // Verlagerung des Codes in seperate Lese/Schreibfunktion sinnvoll
        if (this->reading) { //Empfange Messprogramm
            if (Serial.available() > 0) {
                int errCode = this->readLine();
                if (errCode == 1) { //Funktion wird ausgefuerhrt und bei Erfolg in If gegangen
                    int arraySize = this->splitLine();

                    //for (int i = 0; i < arraySize; i++) {
                    //    Serial.println(this->inDataArray[i]);
                    //}

                    if (this->headerLineCounter == 0) { //ZEILE 1: MFC+Ventilanzahl
                        this->amount_MFC   = atoi(this->inDataArray[0]);
                        this->amount_valve = atoi(this->inDataArray[1]);

                        //erstelle MFC-Objekte
                        for (int i = 0; i < this->amount_MFC; i++) {
                            mfc_list[i] = new control::MfcCtrl(i);
                            main_thread_list -> add_thread(mfc_list[i]);
                        }
                    }

                    this->headerLineCounter++;
                } else {
                    //TODO: ErrorCode muss verarbeitet werden (Displayanzeige?)
                }
            }
        }

        if (this->sending) { //Sende Messwerte parallel zur Messung

        }

        return true;
    }
}
