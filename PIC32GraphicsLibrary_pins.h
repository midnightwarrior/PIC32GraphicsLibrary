#define OUT 0
#define IN 1

#define PowerLED_DIR TRISFbits.TRISF0
#define PowerLED LATFbits.LATF0

#define RST_DIR TRISDbits.TRISD6
#define RST LATDbits.LATD6
#define CS1_DIR TRISBbits.TRISB6
#define CS1 LATBbits.LATB6
#define DB6_DIR TRISDbits.TRISD11
#define DB6 LATDbits.LATD11

#define DB4_DIR TRISEbits.TRISE2
#define DB4 LATEbits.LATE2
#define DB2_DIR TRISEbits.TRISE3
#define DB2 LATEbits.LATE3
#define DB0_DIR TRISEbits.TRISE4
#define DB0 LATEbits.LATE4
#define RW_DIR TRISBbits.TRISB15
#define RW LATBbits.LATB15

#define EN_DIR TRISBbits.TRISB3
#define EN LATBbits.LATB3
#define DI_DIR TRISBbits.TRISB4  //the RS pin
#define DI LATBbits.LATB4
#define DB1_DIR TRISDbits.TRISD9
#define DB1 LATDbits.LATD9
#define DB3_DIR TRISDbits.TRISD10
#define DB3 LATDbits.LATD10
#define DB5_DIR TRISEbits.TRISE5
#define DB5 LATEbits.LATE5
#define DB7_DIR TRISDbits.TRISD8
#define DB7 LATDbits.LATD8
#define CS2_DIR TRISBbits.TRISB7
#define CS2 LATBbits.LATB7

#define DB0_Read PORTEbits.RE4
#define DB1_Read PORTDbits.RD9
#define DB2_Read PORTEbits.RE3
#define DB3_Read PORTDbits.RD10
#define DB4_Read PORTEbits.RE2
#define DB5_Read PORTEbits.RE5
#define DB6_Read PORTDbits.RD11
#define DB7_Read PORTDbits.RD8