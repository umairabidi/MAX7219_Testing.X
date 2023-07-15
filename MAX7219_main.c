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
#define PULSETIME 50

void SPI_send(uint8_t addr, uint8_t data);
void MAX7219_Setup();

void turnOn(int digit_X);
void decimal(int digit_X, int val);
void blink(int digit_X);
void blank(int digit_X);
void blankAll();
void digTest();
void setIntensity();

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
    
    int i;
    SPI_send(0x01,0x86);        // Digit
    SPI_send(0x02,0x85);        // Digit
    SPI_send(0x03,0x84);        // Digit
    SPI_send(0x04,0x83);        // Digit
    SPI_send(0x05,0x82);        // Digit
    
        
    while(1){
    }
    return 0;
} 

void SPI_send(uint8_t addr, uint8_t data){
    int i;
    CS = 0;
    for (i=0; i<8; i++){
        CLK = 0;
        __delay_us(PULSETIME);
        DIN = (addr & 0b10000000) ? 1:0 ;
        __delay_us(PULSETIME);
        CLK = 1;
        __delay_us(PULSETIME);
        addr = addr << 1;
    }
    for (i=0; i<8; i++){
        CLK = 0;
        __delay_us(PULSETIME);
        DIN = (data & 0b10000000) ? 1:0 ;
        __delay_us(PULSETIME);
        CLK = 1;
        __delay_us(PULSETIME);
        data = data << 1;
    }
    CLK = 0;
    CS = 1;
}

void MAX7219_Setup(){
    SPI_send(0x0F,0x00);        // Display Test
    SPI_send(0x09,0xFF);        // Decode mode ON
    SPI_send(0x0A,0x03);        // Intensity level
    SPI_send(0x0B,0x07);        // Scan-Limit
    SPI_send(0x0C,0x01);        // Shutdown (turn it on)
    
}
void turnOn(int digit_X){
    
}
void decimal(int digit_X, int val){
    
}
void blink(int digit_X){
    
}
void blank(int digit_X){
    
}
void blankAll(){
    
}
void digTest(){
    
}
void setIntensity(){
    
}