// ---------------------------------------------------------------------------------------
//        CooCox CoOS Blinker Example  (LPC1769 LPCXpresso evaluation board))
//
// Purpose: Blink LED2 (P0.22) using kernel delay functions.
//
//			This is an attempt to create the simplest possible CoOS example
//          for the LPC1769 LPCXpresso board.
//
//			Everything needed to blink LED2 (Port 0, Bit 22) is in the function main().
//
//			The main() function initializes the LPCXpresso board, initializes the CoOS real
//          time kernel, creates a single task0, and starts the CoOS kernel.
//
//			task0 runs as an endless loop wherein the LED2 is turned on and off in a
//          cyclic pattern controlled by kernel tick delay functions.
//
// ---------------------------------------------------------------------------------------


#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"
#include "CoOs.h"

#define STACK_SIZE_DEFAULT	512
#define PORT0				0
#define INPUT				0
#define OUTPUT				1
#define LED_PINS			(uint32_t)1<<22

// Set up a 512 byte stack for task1
OS_STK task1_stk[STACK_SIZE_DEFAULT];


// -------------------------------------------------------------------
//  initializeBoard() - Sets up the GPIO port 0 to control LED2
//
//                      Turns on the peripheral clock for GPIO Port0
//                      Sets bit 22 of GPIO Port0 to be an output
// -------------------------------------------------------------------
void initializeBoard(){

    // enable GPIO Clock
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

    // LED2 on PORT0.22 defined as Output
    GPIO_SetDir(PORT0, LED_PINS, OUTPUT);

}


// ------------------------------------------------------------------------
//  task1( ) - Blinks LED2 at a two second rate.
//
//             Most CoOS tasks are coded as endless loops.
//
//             This endless loop sets and clears GPIO port0, bit22.
//
//			   A one second CoTickDelay() function after each LED2
//             state change gives the two second repetition rate.
//
//             Note: The current default CoOS tick rate is 10 milliseconds.
//                   Thus 100 * 10msec = 1000msec or 1.0 seconds.
// ------------------------------------------------------------------------
void task1 (void* pdata){
	while(1){
	    // turn OFF LED2
		GPIO_ClearValue(PORT0, LED_PINS);

		// delay (100 * 10msec/tick = 1000 msec)
		CoTickDelay(100);

		// turn ON LED2
		GPIO_SetValue(PORT0, LED_PINS);

		// delay (100 * 10msec/tick = 1000 msec)
		CoTickDelay(100);
	}
}


// ------------------------------------------------------------------------
//  main( ) - Main program creates task0 and then starts CoOS kernel.
//
// ------------------------------------------------------------------------
int main(void)
{
	// initialize the LPCXpresso board by:
	// 		turning on clock signal for the GPIO peripheral.
	// 		setting GPIO port 0, bit22 for output (to drive LED2)
	initializeBoard();

	// initialize the CoOS real time kernel
	CoInitOS();

	// create task1
	CoCreateTask(task1,0,0,&task1_stk[STACK_SIZE_DEFAULT-1],STACK_SIZE_DEFAULT);

	// start the CoOS real time kernel
	CoStartOS();

	// you should never get here!
    while(1);
}
