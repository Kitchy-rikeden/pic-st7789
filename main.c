#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT1  // Power-up default value for COSC bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = OFF      // Debugger enable bit (Background debugger disabled)
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/VPP must be used for programming.)
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

#include <xc.h>
#include "spi.h"
#include "st7789.h"
#include "usart18857.h"

#define _XTAL_FREQ 32000000

const int RGBW[] = {0xF800, 0x07E0, 0x001F, 0xFFFF};

void Init();

void Color(int c){              //16bit = 1px の色を送信
    Write_Parameter(c >> 8);    //上位8bit
    Write_Parameter(c);         //下位8bit
}

void Clear(){                   //全体に白を描画
    for(char i=0; i<240; i++)
        for(char j=0; j<240; j++)
            Color(0xFFFF);
    address();
}

void __interrupt isr(){
    Write_Parameter(EUSART_Receive());
}

void main(void) {
    Init();
    SPI_Init();
    EUSART_Init();
        
    Initial_ST7789();
    address();
    Clear();
    
    char c = 0;
    for(char k=0; k<240; k++)           //サンプルパターン
        for (char j=0; j<4; j++)
            for(char i=0; i<60; i++)
                Color(RGBW[j]);
    
    while(1);
}

void Init(){
    OSCCON1 = 0b00000000;   //32MHz
        
    TRISA  = 0b00100000;
    ANSELA = 0b00000000;
    
    TRISC  = 0b00010000;
    ANSELC = 0b00000000;
    WPUC   = 0b00000000;
    
    PEIE = 1;
    GIE = 1;
}
