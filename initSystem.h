// initSystem.h - part of the PIC32GraphicsLibrary.h set of libraries
// Copyleft midnightwarrior, 2015

// Description: This includes extra instructions to set up your particular system.
// In my case I've set the system clock at 80MHz and the "tick" to 40 ticks/cycle,
// and set up hardware timer 1 to delay for 5000 ticks (or 1us).

#define	SYS_FREQ	(80000000L)
#define CORE_TICK_RATE 40

void initSystem(void) {
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    OpenCoreTimer(CORE_TICK_RATE);
    // Set up timer 1 as the basis for the delay_us() function
    OpenTimer1(T1_ON | T1_IDLE_CON | T1_PS_1_8 | T1_SOURCE_INT, 5000);
}