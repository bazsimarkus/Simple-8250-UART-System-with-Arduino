# Simple 8250 UART System

This circuit provides a simple test environment for a **8250 Universal asynchronous receiver-transmitter (UART) ** chip. In this specific example I used an *NS8250-B* chip, but this software is forward compatible with all of the serial IC-s based on the 8250 architecture. 

This system was intended to be an introductory circuit to the world of UARTs, so I tried to minimize the complexity of the circuit, with keeping the amount of components as low as possible.

## Breadboard photo

![enter image description here](https://github.com/bazsimarkus/Simple-8250-UART-System-with-Arduino/raw/master/images/8250_breadboard.jpg)

## Components needed

 - 8250 UART chip
 - Arduino board (Uno, Nano, both works)
 - 1.8432 MHz crystal
 - USB-TTL Serial converter for testing (FTDI will do as well)
 - 2x 33pF ceramic capacitors
 - 2x 470Ω resistors
 - 2x strandard THT LEDs
	> The USB-TTL Serial adapter is needed to test if the circuit is working properly. Check out "Testing".

## Working

The circuit works like the following: The Arduino sends 8-bit ASCII characters to the UART chip, then the chip converts it to serial data. The parameters like the baudrate, parity bit, etc are also initialized by the Arduino in the beginning of the procedure. 
The text to send is stored in the PROGMEM section of the microcontroller memory, therefore the Arduino is able to send really long messages wihtout any stack problem. 
*(If we stored the message in a long string, the memory space would be gone really quickly)*

I had to use an 1.8432 MHz crystal in order to be able to achieve the desired standard 9600 Baud rate. So with dividing the clock frequency with 12 (set in the divisor register, see code), the communication speed will be compatible with most of the terminals.

## Using

 1. Wire up the circuit based on schematics
 2. Upload the 8250.ino source code to the Arduino board
 3. Connect the USB-TTL Serial converter to PC
 4. Open a serial terminal program (for example RealTerm)
 5. Enjoy!

## Schematic

![enter image description here](https://github.com/bazsimarkus/Simple-8250-UART-System-with-Arduino/raw/master/images/8250_sch.png)

## Testing

For testing get a serial terminal program like RealTerm:
https://realterm.sourceforge.io/

Then select the COM port of your USB-TTL Serial converter, and open up the serial port to see the results.

## Future goals

 - It's a good idea to add a MAX232 chip to make the circuit fully compatible with the RS232 standard!

## Development

The project was developed using Arduino IDE 1.6.9 (https://www.arduino.cc/) and the testing was made with RealTerm (https://realterm.sourceforge.io/)

## Notes

Check out my other serial project with the 6850 ACIA chip, which was developed parallel with this circuit!
