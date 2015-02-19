// PIC32 Graphics Library
// Copyleft midnightwarrior, 4 February 2015
// Initial public release: v15.02b04a
// Version v15.02b04a

//    This program is free software; you can redistribute it and/or
//    modify it under the terms of the GNU General Public License
//    as published by the Free Software Foundation; either version 2
//    of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
        
// Usage: This library lets the user easily use a KS0107/8 dot-matrix display with their
// PIC32 project.
// Any pin can be connected to anything - you don't have to use a single chip port for, say,
// all of the data pins for example. Flexibility, not efficiency, was the purpose here so this
// approach may actually be less efficient than other available graphics libraries. That said, it's
// still very useable.

// To customise for your project, just modify the the pin definitions in PIC32GraphicsLibrary_pins.h

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <plib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "PIC32GraphicsLibrary_pins.h"
#include "prototypes.h"
#include "font.h"

#define PI 3.141592654

void Enable_Pulse(void);
void Draw_Point(unsigned short, unsigned short, unsigned short);
void GLCD_CLR(void);
void GLCD_Clrln(unsigned short);
void GLCD_Data(BYTE);
void GLCD_FlashRapidly(void);
void GLCD_ON(void);
unsigned short GLCD_Read(unsigned short);
void GLCD_Write(BYTE);
void GOTO_COL(unsigned int);
void GOTO_ROW(unsigned int);
void Set_Start_Line(unsigned short);
void GOTO_XY(unsigned int, unsigned int);
void delay_us(WORD);
void setDataBusAsRead(void);
void initLCD(void);


void initLCD(void) {
    // Initialises the PIC32's GPIOs
    AD1PCFG = 0xFFFF;
    CS1_DIR = OUT;
    CS2_DIR = OUT;
    RST_DIR = OUT;
    RW_DIR = OUT;
    DB0_DIR = OUT;
    DB1_DIR = OUT;
    DB2_DIR = OUT;
    DB3_DIR = OUT;
    DB4_DIR = OUT;
    DB5_DIR = OUT;
    DB6_DIR = OUT;
    DB7_DIR = OUT;
    EN_DIR = OUT;
    DI_DIR = OUT;
}

void setDataBusAsRead(void) {
    // Set tristate registers so the LCD's data bus can get read
    // This function DOESN'T ACTUALLY WORK PROPERLY! - fix!
    DB0_DIR = IN;
    DB1_DIR = IN;
    DB2_DIR = IN;
    DB3_DIR = IN;
    DB4_DIR = IN;
    DB5_DIR = IN;
    DB6_DIR = IN;
    DB7_DIR = IN;
    //PowerLED = DB0_DIR;
    //delay_us(20000);
}

void GLCD_DrawLine(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned short style) {
    // Draw a line on the screen!

    // Bresenham's Line Algorithm - from http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
      switch(style) {
          case 0: // Solid line
              Draw_Point(x0,y0,1);
              break;
          case 1: // Dotted line - even pixels
              if(!(x0 % 2)) {
                   Draw_Point(x0,y0,1);
                   PowerLED = 1;
              }
              break;
          case 2: // Dotted line - odd pixels
              if(x0 % 2) {
                  Draw_Point(x0,y0,1);
              }
              break;
      }
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) {
        err -= dy;
        x0 += sx;
    }
    if (e2 < dy) {
        err += dx;
        y0 += sy;
    }
  }
}

void GLCD_DrawCircle(unsigned short x, unsigned short y, unsigned short r, unsigned short colour) {
    // Draw a circle!
    int angle;
    for(angle=0; angle<360; angle++) {
        Draw_Point(x + r*cos(angle*(PI/180)), y + r*sin(angle*(PI/180)), colour);
    }
}
void GLCD_DrawBox(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short fillpattern) {
    // Code to draw a box and fill/unfill/hatch/whatever it
    int x, y;
    // Swap coords around if need be
    if(x1 > x2) {
        x = x1;
        x1 = x2;
        x2 = x;
    }

    if(y1 > y2) {
        y = y1;
        y1 = y2;
        y2 = y;
    }

    // Shading routine
    switch(fillpattern) {
        case 1: // Fill the box
            for(y=y1; y<y2+1; y++) {
                GLCD_DrawLine(x1, y, x2, y, 0);
            }
            break;
        case 2: // Dot teh box
            for(y=y1+1; y<y2; y+=2) {
                GLCD_DrawLine(x1+1, y, x2, y, 1);
                GLCD_DrawLine(x1+1, y+1, x2, y+1, 2);
            }
            break;
    }

    GLCD_DrawLine(x1, y1, x1, y2, 0);
    GLCD_DrawLine(x1, y1, x2, y1, 0);
    GLCD_DrawLine(x2, y1, x2, y2, 0);
    GLCD_DrawLine(x1, y2, x2, y2, 0);
}

void Enable_Pulse() {
    EN = 1;
    // Delay 5us
    delay_us(5);
    EN = 0;
    // Delay 5us
    delay_us(5);
}

void GLCD_ON() {
    CS1 = 0;
    CS2 = 0;
    RST = 1;

    // Switch on both chips
    CS1 = 1;
    CS2 = 1;
    DI = 0;
    //RW = 1;
    RW = 0;
    GLCD_Data(0x3F);
    Enable_Pulse();
    return;
}

void GLCD_OFF() {
    // Switch on both chips
    CS1 = 1;
    CS2 = 1;
    DI = 0;
    //RW = 1;
    RW = 0;
    GLCD_Data(0x3E);
    Enable_Pulse();
    return;
}

void Set_Start_Line(unsigned short line) {
    DI = 0;
    RW = 0;
    CS1 = 1;
    CS2 = 1;
    GLCD_Data(0xC0 | line);
    Enable_Pulse();
}

void GOTO_COL(unsigned int x)
{
    unsigned short Col_Data;

    DI = 0;
    RW = 0;
    if(x<64) {
        CS1 = 1;
        CS2 = 0;
        Col_Data = x;
    }
    else {
        CS1 = 0;
        CS2 = 1;
        Col_Data = x-64;
    }
    Col_Data = (Col_Data | 0x40) & 0x7F;
    GLCD_Data(Col_Data);
    Enable_Pulse();
}

void GOTO_ROW(unsigned int y) {
    unsigned short Row_Data;
    DI = 0;
    RW = 0;
    Row_Data = (y | 0xB8) & 0xBF;
    GLCD_Data(Row_Data);
    Enable_Pulse();
}

void GOTO_XY(unsigned int x, unsigned int y) {
    GOTO_ROW(y);
    GOTO_COL(x);
}

void GLCD_Write(BYTE b) {
    DI = 1;
    RW = 0;
    GLCD_Data(b);
    delay_us(1);
    Enable_Pulse();
}

unsigned short GLCD_Read(unsigned short column) {
    unsigned short read_data = 0;
    // Set ports as inputs
    // This requires magic with tristate buffers
    // EDIT: Magic has already been done in a routine!

    setDataBusAsRead();

    DI = 1;
    RW = 1;

    CS2 = (column>63);
    CS1 = !CS2;

    // Delay 1us
    delay_us(1);
    EN = 1;

    // Delay 1us
    delay_us(1);
    EN = 0;

    // Delay 5us
    delay_us(5);
    EN = 1;
    // Delay 1us
    delay_us(1);
    // Get data off the port!

    read_data = (DB7_Read * 128) + (DB6_Read * 64) + (DB5_Read * 32) + (DB4_Read * 16)
            + (DB3_Read * 8) + (DB2_Read * 4) + (DB1_Read * 2) + (DB0_Read);
    EN = 0;
    // Delay 1us
    delay_us(1);

    initLCD();
    return read_data;
}

void GLCD_Clrln(unsigned short ln) {
    int i;
    GOTO_XY(0,ln); //At start of line of left side
    GOTO_XY(64,ln); //At start of line of right side (Problem)
    // Enable CS1
    CS1 = 1;
    for(i=0;i<65;i++) {
        GLCD_Write(0x00);
    }
}

void GLCD_CLR() {
    unsigned short m;
    for(m=0;m<8;m++){
        GLCD_Clrln(m);
    }
}

void Draw_Point(unsigned short x,unsigned short y, unsigned short color) {
    unsigned short Col_Data;
    GOTO_XY(x,(y/8));
    switch (color) {
    case 0: //Light spot
        Col_Data = ~(1<<(y%8)) & GLCD_Read(x);
        break;
    case 1: //Dark spot
        Col_Data = (1<<(y%8)) | GLCD_Read(x);
        break;
    }
    GOTO_XY(x,(y/8));
    GLCD_Write(Col_Data);
}

void writeByteToScreen(unsigned short x,unsigned short y, unsigned short color) {
    unsigned short Col_Data;
    switch (color) {
    case 0: //Light spot
        Col_Data = ~(1<<(y%8));
        break;
    case 1: //Dark spot
        Col_Data = (1<<(y%8));
        break;
    }
    GOTO_XY(x,(y/8));
    GLCD_Write(Col_Data);
}

void GLCD_Data(BYTE data) {

    // Write data!
    DB0 = ((data >> 0) & 0x01);
    DB1 = ((data >> 1) & 0x01);
    DB2 = ((data >> 2) & 0x01);
    DB3 = ((data >> 3) & 0x01);
    DB4 = ((data >> 4) & 0x01);
    DB5 = ((data >> 5) & 0x01);
    DB6 = ((data >> 6) & 0x01);
    DB7 = ((data >> 7) & 0x01);

    return;
}


void delay_us(WORD delay)
 {

    // New approach! - use timer 1
    // Timer 1 is configged to have a resolution of 1/5000 ms - that's 0.2us or
    // 200ns.  Probably a bit overkill, but meh.
    // This function will set the maximum value of the timer to the required period

    unsigned timerResetValue = delay * 5;
    unsigned int int_status;

    int_status = INTDisableInterrupts();
    OpenTimer1(T1_ON | T1_IDLE_CON | T1_PS_1_8 | T1_SOURCE_INT,  timerResetValue);
    INTRestoreInterrupts(int_status);

    WriteTimer1(0);

    while(1) {
        if(mT1GetIntFlag()) {
            mT1ClearIntFlag();
            return;
        }
    }
}

void GLCD_RenderText(int init_x, int init_y, char *str) {
    int a, b, x, y, offset = 0;
    GOTO_XY(init_x, init_y);

    // Loop through string and print chars
    while(offset < strlen(str)) {
        x = offset * 4 + init_x;
        y = init_y;
        GOTO_XY(x, y);
        for(a=0; a<3; a++) {
            for(b=5; b+1>0; b--) {
                Draw_Point(x + a, y + b, (((littlenumbers[str[offset] - 0x20][a]) >> b) & 0x01));
            }
        }
        offset++;
    }
}

void GLCD_RenderText_writeBytes(int init_x, int init_y, char *str, int colour) {
    // NOTE: y position here is the page, not the y pixel position
    // Extra parameter - colour - 1 means text is white with black background
    // 0 means black text on white background
    // Runs like greased lightning in comparison to the other RenderText function!
    int a, b, x, y, offset = 0;
    GOTO_XY(init_x, init_y);
    GOTO_ROW(init_y);
    if(!colour) {
        GLCD_Write(0xFF);
    }

    // Loop through string and print chars
    while(offset < strlen(str)) {
        if(!colour) {
            x = offset * 6 + init_x + 1;
        }
        else if(colour) {
            x = offset * 6 + init_x;
        }
        y = init_y;
        GOTO_XY(x, y);
        GOTO_ROW(init_y);
        for(a=0; a<5; a++) {
            GOTO_XY(x + a, y);
            GOTO_ROW(init_y);
            if(colour) {
                GLCD_Write((font[str[offset] - 0x20][a]));
            }
            else if(!colour) {
                GLCD_Write(~(font[str[offset] - 0x20][a]));
            }
        }
        offset++;
        if(colour) {
            GLCD_Write(0x00);
        }
        else if(!colour) {
            GLCD_Write(0xFF);
        }
    }
}

char * strtok_single (char * str, char const * delims)
{
  static char  * src = NULL;

  char  *  p,  * ret = 0;

  if (str != NULL)
    src = str;

  if (src == NULL)
    return NULL;


  if ((p = strpbrk (src, delims)) != NULL) {
    *p  = 0;
    ret = src;
    src = ++p;
  }

  return ret;
}

// Define array to hold "text window" contents - this is for the scrolling terminal
// function, added 20 February 2015

char textWindow[7][21];

void initScrollingTerminal(void) {
    int a,b;
    for(a=0;a<7;a++) {
        for(b=0;b<21;b++) {
            textWindow[a][b] = '\0';
        }
    }
}

void scrollingTerminal(char *lineContents) {
    // This function implements a text window with scrollback, for use with something
    // like a text terminal.

    int lineNumber, a, b = 0;
    // Find out how many lines are already on the screen
    if(textWindow[0][0] == '\0') {
        // Copy lineContents to textWindow[0]
        for(a=0;a<20;a++) {
            textWindow[0][a] = lineContents[a];
        }
        displayScrollingTerminal();
        return;
    }
    // Starting location is *not* zero - find it!
    for(a=0;a<7;a++) {
        if(textWindow[a][0] != '\0') {
            // Not null character - there's data here!
            lineNumber = a;
            break;
        }
    }
    if(a==0) {
        // Buffer is full - starting location of zero has already been handled
        // Push everything up one line
        for(lineNumber=0;lineNumber<6;lineNumber++) {
            for(b=0;b<20;b++) {
                textWindow[lineNumber][b] = textWindow[lineNumber+1][b];
            }
        }
        for(b=0;b<20;b++) {
            textWindow[7][b] = '\0';
        }
        lineNumber = 6;
    }
    // Buffer is not full - line to use is at lineNumber
    for(b=0;b<20;b++) {
        textWindow[lineNumber][b] = lineContents[b];
    }
    displayScrollingTerminal();
    return;
}

void displayScrollingTerminal(void) {
    // Display updated scrolling buffer on the screen
    int lineNumber = 0;
    for(lineNumber=0;lineNumber<6;lineNumber++) {
        GLCD_RenderText_writeBytes(0,lineNumber,"                      ",1);
        GLCD_RenderText_writeBytes(0,lineNumber,textWindow[lineNumber],1);
    }
}
