#ifndef config_h
#define config_h

// In dieser Datei werden Hardgecodete Einstellungen vorgenommen

#define SERIAL_LABVIEW_BAUDRATE 250000
#define SERIAL_DEBUG_BAUDRATE 250000
#define SERIAL_UART_BAUDRATE 250000

#define SERIAL_READ_TIMEOUT 1000 //Zeit in ms, die eine Zeilenuebertragung maximal beanspruchen darf
#define SERIAL_READ_MAX_LINE_SIZE 512 //Maximale Laenge einer uebertragenenen Zeile
#define SERIAL_READ_MAX_BLOCK_SIZE 16 //Maximale Laenge eines Eintrages (Zerlegter Uebertragungsstring)
#define SERIAL_READ_MAX_BLOCK_AMOUNT 32 //Maximale Anzahl an Eintraegen pro Zeile

#define MAX_AMOUNT_MFC 16
#define MAX_AMOUNT_VALVE 16

//ERORRORCOES
#define ERR_SERIAL_READ_MAX_STRING_SIZE 1001
#define ERR_SERIAL_READ_WRONG_LINE_BEGIN 1002
#define ERR_SERIAL_READ_WRONG_LINE_ENDING 1003
#define ERR_SERIAL_READ_TIMEOUT 1004

#endif
