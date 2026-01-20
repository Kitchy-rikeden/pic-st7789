#include<xc.h>

#define _XTAL_FREQ 32000000
#define BAUDRATE 115200

void EUSART_Init(){
    TX1STA = 0x20;
    RC1STA = 0x90;
    BAUD1CON = 0x08;
    SP1BRG = (_XTAL_FREQ/BAUDRATE + 1)/16;
    RCIE = 1;
    RCIF = 0;
    
    RXPPS = 0x05;       // RX input
    RA4PPS = 0x14;      // TX output
}

void EUSART_Send(char c){
    while(TRMT == 0);
    TX1REG = c;
}

char EUSART_Receive(){
    if(RCIF == 1){
        if((RC1STAbits.OERR)||(RC1STAbits.FERR)){
            RC1STA = 0;
            RC1STA = 0x90;
            return 0xff;    //ERROR
        }
        else
            return RC1REG;  //NORMAL
    }
    else
        return 0;           //NO DATA
}
