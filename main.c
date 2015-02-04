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
// NOTE: initSystem.h includes

// BUG! - for some reason a vertical line is drawn at the end of the text lines, and I am unsure of the reason.

#include "PIC32GraphicsLibrary.h"
#include "initSystem.h"

// A "Hello World!" application to demonstrate the usage of the libraries follows.

int main(void) {

    // Initialise microcontroller pins
    initLCD();
    CS1 = 0;
    CS2 = 0;
    RST = 1;

    // Switch on screen and clear it
    GLCD_ON();
    GLCD_CLR();

    // Not sure if this strictly necessary but it does no harm adding it I don't think.
    Set_Start_Line(0);

    // Actually do stuff
    int a, screenState = 0;
    while(1) {
        for(a=0;a<8;a++) {
            GLCD_RenderText_writeBytes(0,a,"Greetings world!",screenState);
            screenState = !screenState;
        }
        // Change text state to white on black background
        screenState = 1;
        // Delay 1000ms
        for(a=0;a<40;a++) {
            delay_us(50000);
        }
        for(a=0;a<8;a++) {
            GLCD_RenderText_writeBytes(0,a,"Greetings world!",screenState);
            screenState = !screenState;
        }
        // Change text state to black on white background
        screenState = 0;
        // Delay 1000ms
        for(a=0;a<40;a++) {
            delay_us(50000);
        }
    }
    // Prevents the program from terminating
    while(1);
}