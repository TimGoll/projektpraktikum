#include "main_labCom.h"

namespace communication {
    Main_LabCom::Main_LabCom() {
        this->reading = true;
        this->sending = false;
        this->finished = false;

        this->amount_valve = 0;
        this->amount_MFC = 0;

        this->headerLineCounter = 0;

        srl->println('D', "LabCom erstellt.");
        srl->println('L', "ready"); //Sende Startbefehl an LabView
    }
    Main_LabCom::~Main_LabCom() {

    }

    void Main_LabCom::setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl) {
        this->main_mfcCtrl = main_mfcCtrl;
    }

    void Main_LabCom::setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl) {
        this->main_valveCtrl = main_valveCtrl;
    }

    void Main_LabCom::setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom) {
        this->main_boschCom = main_boschCom;
    }


    void Main_LabCom::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    void Main_LabCom::setMainStringBuilderObjectPointer(communication::Main_StringBuilder *main_stringBuilder) {
        this->main_stringBuilder = main_stringBuilder;
    }

    int16_t Main_LabCom::readLine() {
        this->bufferCharIndex = 0; //setze index auf Startposition zurueck
        uint32_t startTime = millis();

        //Lese so lange, bis Zeile vollstaendig empfangen wurde, oder Timeout erreicht wurde (Main-Thread wird blockiert)
        while(millis() < startTime + SERIAL_READ_TIMEOUT) {
            if (srl->available('L') > 0) { //Lese Daten, wenn vorhanden
                this->inDataBuffer[this->bufferCharIndex] = srl->read('L'); //srl->read() gibt einen einzelnen Char zurueck

                //UEBERPRUEFUNG
                if (this->bufferCharIndex == 0) {
                    if (this->inDataBuffer[this->bufferCharIndex] != '<') { //erstes Zeichen muss oeffnender Tag sein
                        if (this->inDataBuffer[this->bufferCharIndex] == '\n') //Sortiere Strings ohne Inhalt aus
                            return -1;

                        srl->println('D', "ERROR - Falscher Zeilenbeginn");
                        //lese String dennoch bis Zum Ende um mehrfache "Falscher Beginn" Meldung zu verhindern
                        char searchForEnd = ' ';
                        while (searchForEnd != '\n') {
                            searchForEnd = srl->read('L');
                        }
                        return ERR_SERIAL_READ_WRONG_LINE_BEGIN;
                    }
                }
                if (this->inDataBuffer[this->bufferCharIndex] == '\n') { //Zeile zuende
                    if (this->inDataBuffer[this->bufferCharIndex -1] != '>') { //vorheriges Zeichen muss schliessender Tag sein
                        srl->println('D', "ERROR - Falsches Zeilenende");
                        return ERR_SERIAL_READ_WRONG_LINE_ENDING;
                    } else { //vollstaendiger String abgeschlossen
                        this->inDataBuffer[this->bufferCharIndex +1] = '\0';
                        srl->println('D', "");
                        srl->print('D', "Eingabestring akzeptiert: ");
                        srl->print('D', this->inDataBuffer);
                        return 1;
                    }
                }

                this->bufferCharIndex++;
                if (this->bufferCharIndex == SERIAL_READ_MAX_LINE_SIZE) {
                    return ERR_SERIAL_READ_MAX_STRING_SIZE;
                    srl->println('D', "ERROR - Eingabestring zu lang");
                }
            }
        }
        srl->print('D', "ERROR - Timeout: ");
        srl->println('D', this->inDataBuffer);
        return ERR_SERIAL_READ_TIMEOUT; //Timeout
    }

    uint16_t Main_LabCom::splitLine() {
        uint16_t currentCharIndex = 0;
        uint16_t endCharIndex = this->bufferCharIndex -1; //da letztes Zeichen '>' ist

        //Schleife, die durch die Ziel-Array-Indexe navigiert
        for (uint16_t arrayIndex = 0; arrayIndex < SERIAL_READ_MAX_BLOCK_AMOUNT; arrayIndex++) {
            //Schleife, die durch die Char-Positionen eines Ziel-Array-Eintrags navigiert
            for (uint16_t arrayCharIndex = 0; arrayCharIndex < SERIAL_READ_MAX_BLOCK_SIZE; arrayCharIndex++) {
                currentCharIndex++; //Wird anfangs um 1 erhoet, Zeichen 1 ist zu ignorieren ('<')

                if (this->inDataBuffer[currentCharIndex] == ',') { //Eintrag zu ende
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    cmn::trim(this->inDataArray[arrayIndex]);
                    break;
                }

                if (currentCharIndex == endCharIndex) { //Zeilenende erreicht
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    cmn::trim(this->inDataArray[arrayIndex]);
                    return arrayIndex +1; //Index und Groesse sind immer um 1 verschieden
                }

                this->inDataArray[arrayIndex][arrayCharIndex] = this->inDataBuffer[currentCharIndex];
            }
        }
        return 0;
    }

    void Main_LabCom::start() {
        //Aendere Seriellen Modus
        this->reading = false;
        this->sending = true;

        //Fuege 1000ms zum Start hinzu, um Verzoegerungen durch die Startanzeige zu vermindern
        uint32_t startTime = millis() + 1000;

        srl->println('D', startTime);

        //starte MFCs
        this->main_mfcCtrl->start(startTime);

        //starte Ventile
        this->main_valveCtrl->start(startTime);

        //starte Boschsensor
        this->main_boschCom->start(startTime);

        //starte Display
        this->main_display->start(startTime);

        //starte Stringbuilder (und damit SD)
        this->main_stringBuilder->start(startTime);

        srl->print('D', "[Zeit: ");
        srl->print('D', startTime);
        srl->println('D', "] Messung gestartet.");
    }

    void Main_LabCom::setNewLine(char newLine[]) {
        //Prinzte neue Zeile an LabView
        //TODO!
    }

    //////////////////// MAINLOOP ////////////////////

    bool Main_LabCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        // Im Lesemodus wird, wenn ein String verfügbar ist (srl->available() ), der String
        // vollstaendig eingelesen und, sofern kein Fehler aufgetreten ist, anschließend in ein Array zerteilt.
        // Je nach headerLineCounter (Zeile im Header) wird dieses Array an eine anderen Stelle weiter
        // verarbeitet.
        if (this->reading) { //Empfange Messprogramm
            if (srl->available('L') > 0) {
                int errCode = this->readLine();
                if (errCode == 1) { //Funktion wird ausgefuerhrt und bei Erfolg in If gegangen
                    //Der Header muss einzeln verarbeitet werden, daher gibt es einen headerLineCounter,
                    //der die erwartete Zeile speichert
                    int arraySize = this->splitLine();

                    switch (this->headerLineCounter) {
                        case 0: //ZEILE 0: MFC+Ventilanzahl
                            if (arraySize != 2) {
                                srl->println('D', "Zeile 0: Falsche Anzahl an Eintraegen.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            this->amount_MFC   = atoi(this->inDataArray[0]);
                            this->amount_valve = atoi(this->inDataArray[1]);

                            //erstelle MFC-Objekte in der main_mfcCtrl
                            this->main_mfcCtrl->createMFC(this->amount_MFC);

                            //erstelle Ventil-Objekte in der main_valveCtrl
                            this->main_valveCtrl->createValve(this->amount_valve);

                            //sage Display, dass Uebertragung gestartet wurde
                            this->main_display->header_started(this->amount_MFC, this->amount_valve);

                            this->headerLineCounter = 1;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 1: //ZEILE 1: MFC-Adressen
                            if (arraySize != this->amount_MFC) {
                                srl->println('D', "Zeile 1: Anzahl der gegebenen Adressen stimmt nicht mit Anzahl der MFCs ueberein.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            this->main_mfcCtrl->setAddresses(this->inDataArray);

                            this->headerLineCounter = 2;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 2: //ZEILE 2: MFC-Typen
                            if (arraySize != this->amount_MFC) {
                                srl->println('D', "Zeile 2: Anzahl der gegebenen Typen stimmt nicht mit Anzahl der MFCs ueberein.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            this->main_mfcCtrl->setTypes(this->inDataArray);

                            this->headerLineCounter = 3;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 3: //ZEILE 3: Ventil PCB Adressen
                            if (arraySize == 0 || arraySize > MAX_AMOUNT_VALVE_PCB) {
                                srl->println('D', "Zeile 3: Anzahl der Adressen ist nicht korrekt.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            this->main_valveCtrl->setPcbAddresses(arraySize, this->inDataArray);

                            this->headerLineCounter = 4;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 4: //ZEILE 4: Ventil-Pins
                            if (arraySize != this->amount_valve) {
                                srl->println('D', "Zeile 4: Anzahl der gegebenen Pin-Nummern stimmt nicht mit Anzahl der Ventile ueberein.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            this->main_valveCtrl->setPins(this->inDataArray);

                            this->headerLineCounter = 5;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 5: //ZEILE 5: Messaufloesung wird gesetzt
                            if (arraySize != 1) {
                                srl->println('D', "Zeile 5: Intervallgroesse besteht aus einem Wert.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            //StringBuilder, sowie BoschCom arbeiten mit dem selben Intervall
                            //Ersterer ist jedoch um eine halbe Periode in der Zeit verschoben
                            this->main_boschCom->setIntervall(atoi(this->inDataArray[0]));
                            this->main_stringBuilder->setIntervall(atoi(this->inDataArray[0]));

                            this->headerLineCounter = 6;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 6: //ZEILE 6: DateString wird gesetzt
                            if (arraySize != 1) {
                                srl->println('D', "Zeile 6: Datum besteht aus einem Wert.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            this->main_stringBuilder->setDateString(this->inDataArray[0]);

                            this->headerLineCounter = 7;
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 7: //ZEILE 7: Letzte Zeile, hier wird ein 'begin' erwartet
                            if (strcmp(this->inDataArray[0], "begin") == 0) {
                                srl->println('D', "Header vollstaendig.");

                                //Sage Display, dass Header vollstaendig und Events beginnen
                                this->main_display->event_started();

                                srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                                this->headerLineCounter = 8;
                            } else {
                                srl->println('D', "Zeile 7: Es wird ein 'begin' erwartet");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                            }
                            break;
                        case 8: //ZEILE 8: Eventliste
                            if (arraySize != 4 && strcmp(this->inDataArray[0], "end") != 0) {
                                srl->println('D', "Zeile 8: Events bestehen aus 4 Elementen, ansonsten wird ein 'end' erwartet.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            if (strcmp(this->inDataArray[0], "M") == 0) { //MFC
                                if (atoi(this->inDataArray[1]) < this->amount_MFC) {
                                    this->main_mfcCtrl->setEvent(
                                        atoi(this->inDataArray[1]), //MFC-ID
                                        atoi(this->inDataArray[2]), //value
                                        strtoul(this->inDataArray[3], NULL, 0) //time (uint32_t)
                                    );
                                } else {
                                    this->main_display->throwError(ERR_SERIAL_UNDEFINED_INDEX);
                                    srl->println('L', ERR_SERIAL_UNDEFINED_INDEX); //Sende Errorcode an LabView
                                }
                            } else if (strcmp(this->inDataArray[0], "V") == 0) { //Ventil
                                if (atoi(this->inDataArray[1]) < this->amount_valve) {
                                    this->main_valveCtrl->setEvent(
                                        atoi(this->inDataArray[1]), //Ventil-ID
                                        atoi(this->inDataArray[2]), //value
                                        strtoul(this->inDataArray[3], NULL, 0) //time (uint32_t)
                                    );
                                } else {
                                    this->main_display->throwError(ERR_SERIAL_UNDEFINED_INDEX);
                                    srl->println('L', ERR_SERIAL_UNDEFINED_INDEX); //Sende Errorcode an LabView
                                }
                            }

                            else if (strcmp(this->inDataArray[0], "end") == 0) { //Am ende wechselt labCom in den Sende-Modus
                                srl->println('D', "Uebertragung abgeschlossen.");

                                //Sage Display, dass Event-Uebertragung abgeschlossen ist
                                this->main_display->event_finished();

                                this->headerLineCounter = 9;
                            }
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                        case 9: //ZEILE 9: Warte auf Start (kann auch durch Button aufgerufen werden)
                            if (arraySize != 1) {
                                srl->println('D', "Zeile 9: Es wird ein 'start' erwartet.");
                                srl->println('L', "1004");
                                this->main_display->throwError(1004);
                                break;
                            }
                            if (strcmp(this->inDataArray[0], "start") == 0) {
                                this->start();
                            }
                            srl->println('L', "ok"); //Sende 'Befehl ok' an LabView
                            break;
                    }
                } else if (errCode > 1) {
                    //ErrorCode wird auf Display angezeigt
                    this->main_display->throwError(errCode);
                    srl->println('L', errCode); //Sende Errorcode an LabView
                } //else -1: Keine Eingabe vorhanden (leerer String)
            }
        }

        if (this->sending) { //Sende Messwerte parallel zur Messung
            //ueberpruefe kontinuierlich, ob Eventliste noch Inhalt hat
            if (this->main_mfcCtrl->getQueueFinished() && this->main_valveCtrl->getQueueFinished() && !this->finished) {
                this->finished = true;
                this->main_display->bothQueuesFinished();
                this->main_stringBuilder->bothQueuesFinished();
            }
        }

        return true;
    }
}
