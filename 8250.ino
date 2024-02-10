/**
 * @file 8250.ino
 * @author Balazs Markus
 * @brief Arduino code for interacting with the 8250 UART chip and transmitting a test string with 9600 baud, 8-bit data, no parity, and 1 stop bit, once every second.
 * @details This code provides a simple test environment for an 8250 Universal Asynchronous Receiver-Transmitter (UART) chip. It is specifically tested with an NS8250-B chip but is forward compatible with all serial ICs based on the 8250 architecture.
 *
 * Pinout of the 8250 UART chip (DIP-40):
 * 
 *          ┌───────○───────┐
 *       D0 │1            40│ Vcc
 *       D1 │2            39│ nRI
 *       D2 │3            38│ nDCD
 *       D3 │4            37│ nDSR
 *       D4 │5            36│ nCTS
 *       D5 │6            35│ MR
 *       D6 │7            34│ nOUT1
 *       D7 │8            33│ nDTR
 *     RCLK │9     8250   32│ nRTS
 *      SIN │10    UART   31│ nOUT2
 *     SOUT │11           30│ INTR
 *      CS0 │12           29│ NC
 *      CS1 │13           28│ A0
 *     nCS2 │14           27│ A1
 * nBAUDOUT │15           26│ A2
 *      XIN │16           25│ nADS
 *     XOUT │17           24│ CSOUT
 *      nWR │18           23│ DDIS
 *       WR │19           22│ RD
 *      GND │20           21│ nRD
 *          └───────────────┘
 * 
 * Pin descriptions of the 8250 UART chip:
 * 
 * ----------------------------------------------------------------------
 * | Pin Number | Name     | Type   | Description                       |
 * |------------|----------|--------|-----------------------------------|
 * | 1          | D0       | In/Out | Data Bit 0 (LSB)                  |
 * | 2          | D1       | In/Out | Data Bit 1                        |
 * | 3          | D2       | In/Out | Data Bit 2                        |
 * | 4          | D3       | In/Out | Data Bit 3                        |
 * | 5          | D4       | In/Out | Data Bit 4                        |
 * | 6          | D5       | In/Out | Data Bit 5                        |
 * | 7          | D6       | In/Out | Data Bit 6                        |
 * | 8          | D7       | In/Out | Data Bit 7 (MSB)                  |
 * | 9          | RCLK     | In     | 16x Baud Rate Clock Input         |
 * | 10         | SIN      | In     | Serial Data Input                 |
 * | 11         | SOUT     | Out    | Serial Data Output                |
 * | 12         | CS0      | In     | Chip Select                       |
 * | 13         | CS1      | In     | Chip Select                       |
 * | 14         | nCS2     | In     | Chip Select (Active Low)          |
 * | 15         | nBAUDOUT | Out    | 16x Baud Output (Active Low)      |
 * | 16         | XIN      | -      | Crystal Input                     |
 * | 17         | XOUT     | -      | Crystal Output                    |
 * | 18         | nWR      | In     | Write control (Active Low)        |
 * | 19         | WR       | In     | Write control                     |
 * | 20         | GND      | -      | Ground                            |
 * | 21         | nRD      | In     | Read control (Active Low)         |
 * | 22         | RD       | In     | Read control                      |
 * | 23         | DDIS     | Out    | Driver Disable                    |
 * | 24         | CSOUT    | Out    | Chip Select Out                   |
 * | 25         | nADS     | In     | Address Strobe (Active Low)       |
 * | 26         | A2       | In     | Register Select Bit 2 (LSB)       |
 * | 27         | A1       | In     | Register Select Bit 1             |
 * | 28         | A0       | In     | Register Select Bit 0 (MSB)       |
 * | 29         | NC       | -      | Not Connected                     |
 * | 30         | INTR     | Out    | Interrupt                         |
 * | 31         | nOUT2    | Out    | Output 2 (Active Low)             |
 * | 32         | nRTS     | Out    | Request to Send (Active Low)      |
 * | 33         | nDTR     | Out    | Data Terminal Ready (Active Low)  |
 * | 34         | nOUT1    | Out    | Output 1 (Active Low)             |
 * | 35         | MR       | In     | Master Reset                      |
 * | 36         | nCTS     | In     | Clear to Send (Active Low)        |
 * | 37         | nDSR     | In     | Data Set Ready (Active Low)       |
 * | 38         | nDCD     | In     | Data Carrier Detect (Active Low)  |
 * | 39         | nRI      | In     | Ring Indicator (Active Low)       |
 * | 40         | Vcc      | -      | +5V Positive Supply               |
 * ----------------------------------------------------------------------
 *
 * Components needed for the test circuit:
 * - 8250 UART chip
 * - Arduino board (Uno, Nano, both works, the important thing is that it should operate on 5V voltage)
 * - 1.8432 MHz crystal
 * - USB-TTL Serial converter for testing (FTDI will do as well)
 * - 2x 33pF ceramic capacitors
 * - Optional: 2x 470Ω resistors with standard THT LEDs in series on TX/RX
 *
 * The circuit works as follows:
 * The Arduino sends 8-bit ASCII characters to the UART chip, then the chip converts it to serial data (9600 baud, 8-bit data, no parity, and 1 stop bit).
 * The parameters like the baud rate, parity bit, etc., are initialized by the Arduino at the beginning of the procedure.
 * The text to send is stored in the PROGMEM section of the microcontroller memory, allowing the Arduino to send long messages without any stack problems.
 * An 1.8432 MHz crystal is used to achieve the desired standard 9600 Baud rate.
 * By dividing the clock frequency by 12 (set in the divisor register, see code), the communication speed will be compatible with most terminals.
 *
 * How to use:
 * 1. Connect your Arduino board to the 8250 UART chip based on the included schematics.
 * 2. Upload the 8250.ino source code to the Arduino board.
 * 3. Connect the USB-TTL Serial converter to the PC and to the RD/TD pins of the 8250
 * 4. Open a serial terminal program (for example, RealTerm), select the COM port of your USB-Serial converter, and set up the terinal to use 9600 baud, 8-bit data, no parity, and 1 stop bit.
 * 5. You should be receiving the string "UARTTEST" once every second.
 *
 */

#include <avr/pgmspace.h>
#include <string.h>

// Pin Definitions
#define A0_ADDRESS_PIN    A0   // Address bus bit 0
#define A1_ADDRESS_PIN    A1   // Address bus bit 1
#define A2_ADDRESS_PIN    A2   // Address bus bit 2
#define RDN               A5   // Receive Data Not (Active Low)
#define MR                12   // Master Reset
#define WRN               11   // Write Not (Active Low)
#define CS2N              10   // Chip Select 2 Not (Active Low)
#define D0                2    // Data Bus bit 0
#define D1                3    // Data Bus bit 1
#define D2                4    // Data Bus bit 2
#define D3                5    // Data Bus bit 3
#define D4                6    // Data Bus bit 4
#define D5                7    // Data Bus bit 5
#define D6                8    // Data Bus bit 6
#define D7                9    // Data Bus bit 7
#define LED_OUTPUT        13   // Arduino onboard LED to indicate status

// Timing constants
#define DELAY_SHORT   1
#define DELAY_MEDIUM  50
#define DELAY_LONG    200

// Test string definition
const char testString[] PROGMEM = {"UARTTEST"}; // We store the string in the PROGMEM, this enables us to store very long strings

/**
 * @brief Arduino setup function.
 * 
 * @details This function is called once when the microcontroller is powered on or reset. It is used to initialize 
 * the microcontroller's hardware and peripherals, such as pins, timers, serial communication, etc.
 * 
 * In this project, the setup function initializes the UART (Universal Asynchronous Receiver-Transmitter) 
 * registers for serial communication and sets the pin modes for input and output.
 * 
 * @note The setup function runs only once after power-up or reset. It should be used to perform 
 * initialization tasks and configuration settings.
 */
void setup() {
    pinMode(LED_OUTPUT, OUTPUT);
    initUart(); // Initializing UART
}

/**
 * @brief Arduino main loop function.
 * 
 * @details The loop function is called repeatedly as long as the microcontroller is powered on.
 * It is used to implement the main program logic, perform tasks, and handle events.
 * 
 * In this project, the loop function continuously a string
 * with the 8250 UART using the printStringToUart function.
 * 
 * @note The loop function runs in an infinite loop and should be used to execute the main
 * program logic or perform tasks that need to be repeated continuously.
 */
void loop() {
    // Print the string with the 8250 UART
    printStringToUart(testString);

    // Blink onboard LED
    digitalWrite(LED_OUTPUT, HIGH);
    delay(DELAY_MEDIUM);
    digitalWrite(LED_OUTPUT, LOW);

    // Wait one second
    delay(1000);
}

/**
 * @brief Set all UART pins to output mode.
 * 
 * @details This function sets all UART pins to output mode. 
 * It configures the pins for controlling the UART communication.
 */
void setUartPinsToOutput() {
    // Control pins
    pinMode(RDN, OUTPUT);   // Receive Data Not (Receive Data inverted) - Output
    pinMode(WRN, OUTPUT);   // Write Not (Write inverted) - Output
    pinMode(CS2N, OUTPUT);  // Chip Select 2 - Active Low - Output
    pinMode(MR, OUTPUT);    // Master Reset - Output
    // Data bus
    pinMode(D0, OUTPUT);    // Data Bus D0 - Output
    pinMode(D1, OUTPUT);    // Data Bus D1 - Output
    pinMode(D2, OUTPUT);    // Data Bus D2 - Output
    pinMode(D3, OUTPUT);    // Data Bus D3 - Output
    pinMode(D4, OUTPUT);    // Data Bus D4 - Output
    pinMode(D5, OUTPUT);    // Data Bus D5 - Output
    pinMode(D6, OUTPUT);    // Data Bus D6 - Output
    pinMode(D7, OUTPUT);    // Data Bus D7 - Output
    // Address bus
    pinMode(A0_ADDRESS_PIN, OUTPUT);    // Address Bit 0 - Output
    pinMode(A1_ADDRESS_PIN, OUTPUT);    // Address Bit 1 - Output
    pinMode(A2_ADDRESS_PIN, OUTPUT);    // Address Bit 2 - Output
}

/**
 * @brief Set data bus pins to high impedance (not used).
 * 
 * @details This function sets the data bus pins to high impedance, 
 * but it is not used in the current implementation.
 */
void setDataBusToHighZ() {
    pinMode(D0, INPUT);    // Data Bus D0 - Input (High-Z)
    pinMode(D1, INPUT);    // Data Bus D1 - Input (High-Z)
    pinMode(D2, INPUT);    // Data Bus D2 - Input (High-Z)
    pinMode(D3, INPUT);    // Data Bus D3 - Input (High-Z)
    pinMode(D4, INPUT);    // Data Bus D4 - Input (High-Z)
    pinMode(D5, INPUT);    // Data Bus D5 - Input (High-Z)
    pinMode(D6, INPUT);    // Data Bus D6 - Input (High-Z)
    pinMode(D7, INPUT);    // Data Bus D7 - Input (High-Z)
}

/**
 * @brief Write an address to the address bus.
 * 
 * @details This function writes an address to the address bus pins.
 * 
 * @param address The address to be written to the address bus.
 */
void writeAddressBus(int address) {
    digitalWrite(A0_ADDRESS_PIN, bitRead(address, 0));
    digitalWrite(A1_ADDRESS_PIN, bitRead(address, 1));
    digitalWrite(A2_ADDRESS_PIN, bitRead(address, 2));
}

/**
 * @brief Write data to the data bus.
 * 
 * @details This function sets the data bus pins in bulk. It uses the bitRead
 * function to retrieve each bit of the input parameter.
 * 
 * @param data The data to be written to the data bus.
 */
void writeDataBus(int data) {
    digitalWrite(D0, bitRead(data, 0));
    digitalWrite(D1, bitRead(data, 1));
    digitalWrite(D2, bitRead(data, 2));
    digitalWrite(D3, bitRead(data, 3));
    digitalWrite(D4, bitRead(data, 4));
    digitalWrite(D5, bitRead(data, 5));
    digitalWrite(D6, bitRead(data, 6));
    digitalWrite(D7, bitRead(data, 7));
}

/**
 * @brief Write a character to UART.
 * 
 * @details This function writes an 8-bit character to the UART for transmission.
 * 
 * @param data The character to be transmitted.
 */
void writeCharToUart(int data) {
    writeDataBus(0b00000000);
    writeAddressBus(0b000); // Access the Data Read/Write Register (Register 0 - 0b000)
    delay(DELAY_SHORT);
    digitalWrite(CS2N, LOW);
    delay(DELAY_SHORT);
    digitalWrite(WRN, LOW);
    delay(DELAY_SHORT);
    writeDataBus(data);
    delay(DELAY_SHORT);
    digitalWrite(WRN, HIGH);
    delay(DELAY_SHORT);
    digitalWrite(CS2N, HIGH);
    delay(DELAY_SHORT);
}

/**
 * @brief Print a string to UART.
 * 
 * @details This function prints a string to the UART for transmission.
 * 
 * @param str The string to be transmitted. The string has to be zero terminated!
 */
void printStringToUart(const char* str) {
    char myChar;
    for (int j = 0; j < strlen(str); j++) {
        myChar = pgm_read_byte_near(str + j);
        writeCharToUart(myChar);
    }
}

/**
 * @brief Blink the onboard LED to indicate status.
 * 
 * @details This function blinks the onboard LED to indicate status.
 * It is used for visual indication in the circuit.
 */
void blinkLed() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(LED_OUTPUT, HIGH);
        delay(DELAY_LONG);
        digitalWrite(LED_OUTPUT, LOW);
        delay(DELAY_LONG);
    }
}

/**
 * @brief Initialize UART registers for communication.
 * 
 * @details This function initializes the UART (Universal Asynchronous Receiver-Transmitter) registers 
 * for communication. It configures various registers such as Line Control Register (LCR), Divisor Latch 
 * Low (DLL), and Divisor Latch High (DLH) to set up the baud rate and word length for serial communication.
 * 
 * 8250 Registers:
 * | Offset | Name | Function                | Use                                |
 * |--------|------|-------------------------|------------------------------------|
 * | 0*     | DATA | Data Read/Write Reg.    | Line I/O                           |
 * | 1*     | IER  | Interrupt Enable Reg.   | Enable Tx, Rx, RxError, Modem int. |
 * | 2      | IID  | Interrupt ID Reg.       | ID of highest interrupt source     |
 * | 3      | LCR  | Line Control Reg.       | Line control parameters and Break  |
 * | 4      | MCR  | Modem Control Reg.      | DTR RTS OUTI OUT2 and loopback     |
 * | 5      | LSR  | Line Status Reg.        | TX and RX status (PE FE OE)        |
 * | 6      | MSR  | Modem Status Reg.       | CTS DSR RI RLSD & changes          |
 * | 7      | SCR  | Scratchpad Reg.         | Hold data temporarily              |
 * | 0*     | DLL  | Divisor Latch LSB       | LSB Baudrate divisor               |
 * | 1*     | DLH  | Divisor Latch MSB       | MSB Baudrate divisor               |
 *
 * For a simple test string transmission, we have to set up the following registers:
 * 
 * 1. Access the Line Control Register (Register 3), and set the Baudrate Divisor Latch Access Bit (Bit 7) to 1. This changes Register 0 to "LSB Baudrate divisor register" and Register 1 to "MSB Baudrate divisor register".
 * 2. Access the LSB Baudrate divisor register (Register 0, because we changed the DLAB bit), and set its content (the Baudrate Divisor LSB) to 12 (0Ch = 0b00001100).
 * 3. Access the MSB Baudrate divisor register (Register 1, because we changed the DLAB bit), and set its content (the Baudrate Divisor LSB) to 0 (00h = 0b00000000).
 * 4. Access the Line Control Register (Register 3), and set the Baudrate Divisor Latch Access Bit (Bit 7) back to 0. This changes Register 0 back to "Data Read/Write Register", and Register 1 to "Interrupt Enable Register". Also change Bit 0 to 1 and Bit 1 to 1, in order to set the the word length to 8 bit. 
 * 5. Access the Data Read/Write Register (Register 0, because we changed the DLAB bit back to zero), and write the 8-bit word that needs to be transmitted. After successful writing, the transmission happens immediately. This step can be repeated infinite times.
 *  
 * @note Bit 7 of the LCR controls access to the DATA/IER (=0) or Divisor latches (=1). 
 *       Before setting the baud rate divisor, Bit 7 of the LCR needs to be set to 1, 
 *       and after setting up the baud rate divisor, it should be set back to 0.
 *       Additionally, to set up a simple test string transmission, the word length should 
 *       be set to 8 bits.
 */
void initUart() {
    // Set initial pin configurations
    digitalWrite(LED_OUTPUT, HIGH); // LED ON to indicate the start of the initialization process
    setUartPinsToOutput();
    writeDataBus(0b00000000); // Set data bus pins to low state
    delay(DELAY_MEDIUM);

    // Step 1: Set the Baudrate Divisor Latch Access Bit to 1
    /*
     * LCR - Line Control Register (Read/Write)
     *
     * Controls the output to the Line:
     *
     *   0,1    Word length. (00=5, 01=6, 10=7, 11=8)
     *   2    Stop bits
     *   3,4    Parity control (00, 10=none, 01 = Odd, 11 = Even)
     *   5    Stick parity (0 = normal, 1 = obtuse!)
     *   6    Send Break.
     *   7    DLA, Divisor Latch Access Bit. (1 = BaudRate divisor latch access)
     */

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    writeAddressBus(0b011); // Select LCR (Register 3 - 0b011)
    delay(DELAY_MEDIUM);

    digitalWrite(CS2N, LOW);
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, LOW);
    writeDataBus(0b10000000); // Set the data bus pins using binary representation
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    // Step 2: Set LSB Baudrate divisor to 12 (0Ch = 0b00001100)
    /*
     * Divisor Latch Setting
     * See datasheet Table III for details.
     *
     * Examples:
     * Baud Rate | Divisor | Baud Rate | Divisor
     * ----------|---------|-----------|--------
     *    50    |  900h   |   2400    |  30h
     *   110    |  417h   |   3600    |  20h
     *   150    |  300h   |   4800    |  18h
     *   300    |  180h   |   7200    |  10h
     *   600    |   C0h   |   9600    |  0Ch
     *  1200    |   60h   |  19200    |  06h
     *  1800    |   40h   |  38400    |  03h
     *  2000    |   3Ah   | 115200    |  01h
     *
     * - Baud rate divisors can be calculated by taking the oscillating
     *   frequency (1,843,200 Hz) and dividing by the quantity of the desired
     *   baud rate times the UART clocking factor (16).  Use the following
     *   formula:
     *
     *   divisor = 1843200 / (BaudRate * 16);
     *
     *   1.8432 MHz = 12 divisor (0Ch = 0b00001100) for 9600 baud
     *
     *   Bit order:
     *   0 - Bit 0
     *   1 - Bit 1 ... 7 - Bit 7
     */

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    writeAddressBus(0b000); // Select LSB Baudrate divisor (Register 0 - 0b000)
    delay(DELAY_MEDIUM);

    digitalWrite(CS2N, LOW);
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, LOW);
    writeDataBus(0b00001100); // Set the data bus pins using binary representation
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    // Step 3: Set MSB Baudrate divisor to 0 (00h = 0b00000000)
    /*
     * Bit order:
     * 0 - Bit 8
     * 1 - Bit 9 ... 7 - Bit 15
     */

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    writeAddressBus(0b001); // Select MSB Baudrate divisor (Register 1 - 0b001)
    delay(DELAY_MEDIUM);

    digitalWrite(CS2N, LOW);
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, LOW);
    writeDataBus(0b00000000); // Set the data bus pins using binary representation
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    // Step 4: Set the Baudrate Divisor Latch Access Bit back to 0 and word length to 8 bits
    /*
     * LCR - Line Control Register (Read/Write)
     *
     * Controls the output to the Line:
     *
     *   0,1    Word length. (00=5, 01=6, 10=7, 11=8)
     *   2    Stop bits
     *   3,4    Parity control (00, 10=none, 01 = Odd, 11 = Even)
     *   5    Stick parity (0 = normal, 1 = obtuse!)
     *   6    Send Break.
     *   7    DLA, Divisor Latch Access Bit. (1 = BaudRate divisor latch access)
     */

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    writeAddressBus(0b011); // Select LCR (Register 3 - 0b011)
    delay(DELAY_MEDIUM);

    digitalWrite(CS2N, LOW);
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, LOW);
    writeDataBus(0b00000011); // Set the data bus pins using binary representation
    delay(DELAY_MEDIUM);

    digitalWrite(WRN, HIGH);
    delay(DELAY_MEDIUM);

    // Step 5: Access the Data Read/Write Register for transmitting data - this is done in a separate function
    /*
     * DATA - Data Read/Write Register (Read/Write)
     *
     * When bit 7 of the LCR is 0 the Read and Write registers are accessed.
     */

    blinkLed(); // Indicate the end of initialization
}
