# Simple 8250 UART System

This circuit provides a straightforward test environment for an **8250 Universal Asynchronous Receiver-Transmitter (UART)** chip. Specifically, in this example, an *NS8250-B* chip is utilized, yet the software is designed to be forward compatible with all serial ICs based on the 8250 architecture.

This system serves as an introductory circuit to the world of UARTs, aiming to minimize complexity while keeping the component count low.

## Breadboard Photo

![Breadboard Photo](https://github.com/bazsimarkus/Simple-8250-UART-System-with-Arduino/raw/master/images/8250_breadboard.jpg)

## Components Needed

- 8250 UART chip
- Arduino board (Uno, Nano, both works, the important thing is that it should operate on 5V voltage)
- 1.8432 MHz crystal
- USB-TTL Serial converter for testing (FTDI will also suffice)
- 2x 33pF ceramic capacitors
- Optional: 2x 470Ω resistors with standard THT LEDs in series on TX/RX
	> The USB-TTL Serial adapter is necessary to test if the circuit is functioning correctly. Refer to "How to use" for details.

## How to use

1. Connect your Arduino board to the 8250 UART chip based on the included schematics.
2. Upload the `8250.ino` source code to the Arduino board.
3. Connect the USB-TTL Serial converter to the PC and to the SIN/SOUT pins of the 8250
4. Open a serial terminal program (for example, RealTerm), select the COM port of your USB-Serial converter, and set up the terminal to use 9600 baud, 8-bit data, no parity, and 1 stop bit.
5. You should be receiving the string "UARTTEST" once every second.

## How it works

The circuit operates as follows: The Arduino sends 8-bit ASCII characters to the UART chip, which then converts them to serial data. Parameters such as baud rate and parity bit are initialized by the Arduino at the beginning of the procedure.
The text to be sent is stored in the PROGMEM section of the microcontroller memory, allowing the Arduino to transmit long messages without encountering stack problems.
*(Storing the message in a long string would quickly deplete memory space.)*

An 1.8432 MHz crystal is employed to achieve the standard 9600 Baud rate. By dividing the clock frequency by 12 (as set in the divisor register, see code), the communication speed becomes compatible with most terminals.

### Register Initialization for Basic 9600 Baud 8N1 Serial Transmission

To initialize the UART registers for communication, the Arduino program follows these steps:

1. Access the Line Control Register (Register 3) and set the Baudrate Divisor Latch Access Bit (Bit 7) to 1. This changes Register 0 to "LSB Baudrate divisor register" and Register 1 to "MSB Baudrate divisor register".

2. Access the LSB Baudrate divisor register (Register 0, because the DLAB bit was changed), and set its content (the Baudrate Divisor LSB) to 12 (0Ch = 0b00001100).

3. Access the MSB Baudrate divisor register (Register 1, because the DLAB bit was changed), and set its content (the Baudrate Divisor LSB) to 0 (00h = 0b00000000).

4. Access the Line Control Register (Register 3) and set the Baudrate Divisor Latch Access Bit (Bit 7) back to 0. This changes Register 0 back to "Data Read/Write Register", and Register 1 to "Interrupt Enable Register". Also, change Bit 0 to 1 and Bit 1 to 1 to set the word length to 8 bits.

5. Access the Data Read/Write Register (Register 0, because the DLAB bit was changed back to zero) and write the 8-bit word that needs to be transmitted. After successful writing, the transmission occurs immediately. This step can be repeated infinitely.

*Note: Bit 7 of the LCR controls access to the DATA/IER (=0) or Divisor latches (=1). Before setting the baud rate divisor, Bit 7 of the LCR needs to be set to 1, and after setting up the baud rate divisor, it should be set back to 0. Additionally, to set up a simple test string transmission, the word length should be set to 8 bits.*

## Schematic

![Schematic](https://github.com/bazsimarkus/Simple-8250-UART-System-with-Arduino/raw/master/images/8250_sch.png)

## 8250 Properties

The 8250 chip, also known as the 8250 UART (Universal Asynchronous Receiver/Transmitter), is a popular integrated circuit used for serial communication in early computer systems. It was developed by Intel in the 1970s and became widely adopted due to its simplicity and versatility. The 8250 UART facilitates asynchronous serial communication, allowing data to be transmitted and received one bit at a time without the need for a shared clock signal between the sender and receiver. It has been used in various applications, including connecting peripherals such as modems, mice, and printers to computers, as well as in embedded systems and industrial automation. The 8250 UART has been succeeded by more advanced UART chips, but its legacy continues to influence serial communication standards and protocols.

In this section, some hardware properties of the 8250 UART chip are provided.

### Pinout of the 8250 UART chip (DIP-40)

```plaintext
          ┌───────○───────┐
       D0 │1            40│ Vcc
       D1 │2            39│ nRI
       D2 │3            38│ nDCD
       D3 │4            37│ nDSR
       D4 │5            36│ nCTS
       D5 │6            35│ MR
       D6 │7            34│ nOUT1
       D7 │8            33│ nDTR
     RCLK │9     8250   32│ nRTS
      SIN │10    UART   31│ nOUT2
     SOUT │11           30│ INTR
      CS0 │12           29│ NC
      CS1 │13           28│ A0
     nCS2 │14           27│ A1
 nBAUDOUT │15           26│ A2
      XIN │16           25│ nADS
     XOUT │17           24│ CSOUT
      nWR │18           23│ DDIS
       WR │19           22│ RD
      GND │20           21│ nRD
          └───────────────┘
```

### Pin Descriptions of the 8250 UART chip

| Pin Number | Name     | Type   | Description                       |
|------------|----------|--------|-----------------------------------|
| 1          | D0       | In/Out | Data Bit 0 (LSB)                  |
| 2          | D1       | In/Out | Data Bit 1                        |
| 3          | D2       | In/Out | Data Bit 2                        |
| 4          | D3       | In/Out | Data Bit 3                        |
| 5          | D4       | In/Out | Data Bit 4                        |
| 6          | D5       | In/Out | Data Bit 5                        |
| 7          | D6       | In/Out | Data Bit 6                        |
| 8          | D7       | In/Out | Data Bit 7 (MSB)                  |
| 9          | RCLK     | In     | 16x Baud Rate Clock Input         |
| 10         | SIN      | In     | Serial Data Input                 |
| 11         | SOUT     | Out    | Serial Data Output                |
| 12         | CS0      | In     | Chip Select                       |
| 13         | CS1      | In     | Chip Select                       |
| 14         | nCS2     | In     | Chip Select (Active Low)          |
| 15         | nBAUDOUT | Out    | 16x Baud Output (Active Low)      |
| 16         | XIN      | -      | Crystal Input                     |
| 17         | XOUT     | -      | Crystal Output                    |
| 18         | nWR      | In     | Write control (Active Low)        |
| 19         | WR       | In     | Write control                     |
| 20         | GND      | -      | Ground                            |
| 21         | nRD      | In     | Read control (Active Low)         |
| 22         | RD       | In     | Read control                      |
| 23         | DDIS     | Out    | Driver Disable                    |
| 24         | CSOUT    | Out    | Chip Select Out                   |
| 25         | nADS     | In     | Address Strobe (Active Low)       |
| 26         | A2       | In     | Register Select Bit 2 (LSB)       |
| 27         | A1       | In     | Register Select Bit 1             |
| 28         | A0       | In     | Register Select Bit 0 (MSB)       |
| 29         | NC       | -      | Not Connected                     |
| 30         | INTR     | Out    | Interrupt                         |
| 31         | nOUT2    | Out    | Output 2 (Active Low)             |
| 32         | nRTS     | Out    | Request to Send (Active Low)      |
| 33         | nDTR     | Out    | Data Terminal Ready (Active Low)  |
| 34         | nOUT1    | Out    | Output 1 (Active Low)             |
| 35         | MR       | In     | Master Reset                      |
| 36         | nCTS     | In     | Clear to Send (Active Low)        |
| 37         | nDSR     | In     | Data Set Ready (Active Low)       |
| 38         | nDCD     | In     | Data Carrier Detect (Active Low)  |
| 39         | nRI      | In     | Ring Indicator (Active Low)       |
| 40         | Vcc      | -      | +5V Positive Supply               |

### Register Descriptions for 8250 UART

The 8250 has a number of registers that must be programmed before it can be used. In operation some of these registers are used to indicate status. Additionally the serial card has some extra ports that must be set up correctly. These extra registers are shown below.

| Offset | Name | Function | Use |
|--------|------|----------|-----|
| 0*     | DATA | Data Read/Write Reg. | Line I/O |
| 1*     | IER  | Interrupt Enable Reg. | Enable Tx, Rx, RxError, Modem int. |
| 2      | IID  | Interrupt ID Reg. | ID of highest interrupt source. |
| 3      | LCR  | Line Control Reg. | Line control parameters and Break. |
| 4      | MCR  | Modem Control Reg. | DTR, RTS, OUT1, OUT2 and loopback. |
| 5      | LSR  | Line Status Reg. | Tx and Rx status (PE, FE, OE) |
| 6      | MSR  | Modem Status Reg. | CTS, DSR, RI, RLSD & changes. |
| 7      | SCR  | Scratchpad Reg. | Hold data temporarily |
| 0*     | DLL  | Divisor Latch LOW | LSB of Baudrate divisor |
| 1*     | DLH  | Divisor Latch HIGH | MSB of Baudrate divisor|

(*) _Note: Bit 7 of the LCR controls access to the DATA/IER (=0) or Divisor latches (=1)._


#### Data Register (DATA) - Read/Write

This register contains the outgoing/incoming serial data.
When bit 7 of the LCR is 0, the Data Register and IER Register are accessed.

#### Interrupt Enable Register (IER) - Read/Write

When bit 7 of the LCR is 0, the IE register is accessed. There are four sources of interrupts on the 8250 that can be enabled by setting a 1 in the following bits of IER.

| Bit | Description                              |
|-----|------------------------------------------|
| 0   | Rx data int. enable                      |
| 1   | Tx holding reg. empty int.               |
| 2   | Rx status int. enable (i.e., Parity, Framing, overrun and BREAK enable). |
| 3   | Modem signal change int. enable.         |

#### Interrupt ID Register (IID) - Read Only

Indicates the source of the highest priority interrupt. Encoding is:

| Bit | Description                                |
|-----|--------------------------------------------|
| 0   | Interrupt pending (0 = interrupt, 1 = none pending). |
| 1   | Interrupt ID bit 0                         |
| 2   | Interrupt ID bit 1                         |
| 3-7 | 0                                          |

The interrupt ID is prioritized and the ID byte is encoded as follows, the action to clear the interrupt is shown in brackets [].

- 06: Rx line status (Highest). [read LSR]
- 04: Rx Data. [read Rx Data]
- 02: Tx Data. [write Tx or read IIR]
- 00: Modem status (Lowest). [read MSR]

#### Line Control Register (LCR) - Read/Write

Sets the properties of the serial communication:

| Bit | Description                                         |
|-----|-----------------------------------------------------|
| 0,1 | Word length. (00=5, 01=6, 10=7, 11=8)              |
| 2   | Stop bits                                           |
| 3,4 | Parity control (00, 10=none, 01 = Odd, 11 = Even)  |
| 5   | Stick parity (0 = normal, 1 = obtuse!)             |
| 6   | Send Break.                                         |
| 7   | DLA, Divisor Latch Access Bit. (1 = Baud Rate divisor latch access) |

#### Modem Control Register (MCR) - Read/Write

| Bit | Description                                       |
|-----|---------------------------------------------------|
| 0   | DTR (pin 4) out                                  |
| 1   | RTS (pin 20) out                                  |
| 2   | OUT1, N/C on PC H/W.                              |
| 3   | OUT2, Enable the IRQ4 signal to the PIC(s) on a PC.|
| 4   | Loopback, test 8250 internally.                  |
| 5-7 | 0                                                 |

#### Line Status Register (LSR) - Read Only

| Bit | Description                |
|-----|----------------------------|
| 0   | Data ready.                |
| 1   | OE, Overrun Error.         |
| 2   | PE, Parity Error.          |
| 3   | FE, Framing Error.         |
| 4   | Break Received.            |
| 5   | Tx Holding register empty. |
| 6   | Tx Shift register empty.   |
| 7   | 0                          |

#### Modem Status Register (MSR) - Read Only

| Bit | Description                  |
|-----|------------------------------|
| 0   | Delta CTS.                   |
| 1   | Delta DSR.                   |
| 2   | RI trailing-Edge detect.     |
| 3   | Delta CD.                    |
| 4   | CTS (pin 5) in status.       |
| 5   | DSR (pin 6) in status.       |
| 6   | RI (pin 22) in status.       |
| 7   | CD (Pin 8) in status.        |

#### Scratch Pad Register (SCR) - Read/Write

This 8-bit Read/Write Register does not control the UART in any way. It is intended as a scratchpad register to be used by the programmer to hold data temporarily.

#### Divisor Latch LSB/MSB Registers

Sets the divisor value to determine the Baud Rate of the serial communication. When the Baudrate Divisor Latch Access Bit in the Line Control Register is set to 1, Register 0 changes to "LSB Baudrate divisor register" and Register 1 changes to "MSB Baudrate divisor register", so that the desired divisor can be set. After setting the divisor value, the Baudrate Divisor Latch Access Bit in the Line Control Register has to be set back to zero.  This changes Register 0 back to "Data Read/Write Register", and Register 1 to "Interrupt Enable Register". Refer to the datasheet for details.

**Divisor examples, using a 1.8432 MHz crystal:**

| Baud Rate | Divisor | Baud Rate | Divisor |
|-----------|---------|-----------|---------|
| 50        | 900h    | 2400      | 30h     |
| 110       | 417h    | 3600      | 20h     |
| 150       | 300h    | 4800      | 18h     |
| 300       | 180h    | 7200      | 10h     |
| 600       | C0h     | 9600      | 0Ch     |
| 1200      | 60h     | 19200     | 06h     |
| 1800      | 40h     | 38400     | 03h     |
| 2000      | 3Ah     | 115200    | 01h     |

- Baud rate divisors can be calculated by taking the oscillating frequency (1,843,200 Hz) and dividing by the quantity of the desired baud rate times the UART clocking factor (16). Use the following formula:
  
  `divisor = 1843200 / (BaudRate * 16);`
  
  Example: 1.8432 MHz = 12 divisor (0Ch = 0b00001100) for 9600 baud

Bit order:
- Bit 0 - Bit 0
- Bit 1 - Bit 1
...
- Bit 7 - Bit 7

## Development

The project was developed using the [Arduino IDE](https://www.arduino.cc/), and testing was conducted with [RealTerm](https://realterm.sourceforge.io/).

Here you can see a screenshot of the result:

![Screenshot](https://github.com/bazsimarkus/Simple-8250-UART-System-with-Arduino/raw/master/images/8250_screenshot.png)

## Notes

Check out my other serial test circuit projects, developed concurrently with this circuit!
