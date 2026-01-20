#include <xc.h>

void SPI_Init(){
    SSP1STAT = 0b00000000;  //SMP: middle/ CKE: IdleToActive
    SSP1CON1 = 0b00110000;  //SSPEN/ CKP: Idle High/ SPIMaster Fosc/4
    
    SSP1DATPPS = 0x14;              // SDI : RC4
    RC3PPS = 0x18;                  // SCK : RC3
    RC5PPS = 0x19;                  // SDO : RC5
}

char SPI_Exchange(char data){
    char dumy;
    
    dumy = SSP1BUF;                 //Clear buffer
    SSP1BUF = data;                 //Send
    while(!SSP1STATbits.BF);
    
    return SSP1BUF;                 //Receive
}