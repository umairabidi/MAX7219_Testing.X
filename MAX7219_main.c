/* 
 * File:   main.c
 * Author: Umair Abidi
 *
 * Created on July 12, 2023, 7:23 PM
 */

// Pinout:
// DIN: RC2 p8
// CS:  RC1 p9
// CLK: RC0 p10

#include <xc.h>
#include <stdint.h>
#include "../include/proc/pic16f616.h"

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config IOSCFS = 8MHZ    // Internal Oscillator Frequency Select bit (8 MHz)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR Disabled)

#define _XTAL_FREQ 8000000

#define DIN PORTCbits.RC2
#define CS  PORTCbits.RC1
#define CLK PORTCbits.RC0
#define PULSETIME 1
#define ON 1
#define OFF 0

void SPI_send(uint8_t addr, uint8_t data);
void MAX7219_Setup();
void display(int digit_X, int val);
void display_decimal(int digit_X, int val);
void blink(int digit_X, int frequency);
void blank(int digit_X);
void turnOff();
void turnOn(int digit_X);
void digTest(int state);
void setIntensity(int intensity);


/* Required Functions:
 * Illuminate digit X               turnOn(X)
 * Give digit X a value             display(X, value)
 * Give digit X a decimal point     decimal(X)
 * Blink Digit X                    blink(X)
 * Blank digit X                    blank(X)
 * Blank all digits (shutdown)      blankAll()
 * Illuminate all segments          digTest()
 * Set Brightness?                  setIntensity()
 * 
 
*/

int main(void){
    TRISCbits.TRISC2 = 0;   // Set as output
    TRISCbits.TRISC1 = 0;   // Set as output
    TRISCbits.TRISC0 = 0;   // Set as output
    ANSEL = 0;              // All pins as Digital IO
    CLK = 0;
    CS = 1;
    DIN = 0;
    uint16_t i, temp;
    uint8_t j;
    MAX7219_Setup();
    
    for (i=0; i<1000000; i++){
        temp = i;
        for (j=1; j<=8; j++){
            display(j, temp%10);
            temp/=10;
        }
        //__delay_ms(1);
    }
   
    while(1);
    return 0;
} 

void SPI_send(uint8_t addr, uint8_t data){
    int i;
    CS = 0;
    for (i=0; i<8; i++){
        CLK = 0;
        DIN = (addr & 0b10000000) ? 1:0 ;
        CLK = 1;
        addr = addr << 1;
    }
    for (i=0; i<8; i++){
        CLK = 0;
        DIN = (data & 0b10000000) ? 1:0 ;
        CLK = 1;
        data = data << 1;
    }
    CLK = 0;
    CS = 1;
}

void MAX7219_Setup(){
    SPI_send(0x0F,0x00);        // Display normal operation
    SPI_send(0x09,0xFF);        // Decode mode ON
    SPI_send(0x0A,0x03);        // Intensity level
    SPI_send(0x0B,0x07);        // Scan-Limit
    SPI_send(0x0C,0x01);        // Shutdown (turn it on)
}

void display(int digit_X, int val){
    SPI_send(digit_X, val);
}
void display_decimal(int digit_X, int val){
    SPI_send(digit_X, 0b10000000 | val);
}
void blink(int digit_X, int frequency){
    // Not sure how yet
}
void blank(int digit_X){
    SPI_send(digit_X, 0x0F);
}
void turnOff(){
    SPI_send(0x0C,0x0F);
    // Datasheet gives DIN=XXXXXXXX ?????
}
void turnOn(int digit_X){
    SPI_send(0x0C,0x01);
}
void digTest(int state){
    // If state is 0 or ON, turn off Display Test
    // If state is 1 or OFF, turn on Display Test
    SPI_send(0x0F, 0x00 | !state);
}
void setIntensity(int intensity){
    SPI_send(0x0A, intensity);
}