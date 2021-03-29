#include <avr/pgmspace.h>
#include <string.h>

#define RDN A5
#define MR 12
#define WRN 11
#define CS2N 10 //8250 CS2 pinre

#define D0 2
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

char myChar;
const char testString[16] PROGMEM = {"UARTTEST"};

// the setup function
void setup() {
    pinMode(13,OUTPUT);

    setBusesToOutput();
    setBusesToLow();
    initUart();
    setBusesToLow();
}

// the main loop
void loop() {

//Send out the string to the UART
    for(int j=0; j<16; j++) {
        myChar = pgm_read_byte_near(testString + j);
        writeCharToUart(myChar);
    }

    delay(5000);
}

void writeCharToUart(int data)
{
    setBusesToLow();
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    delay(1);
    digitalWrite(CS2N, LOW);
    delay(1);
    digitalWrite(WRN, LOW);
    delay(1);

    writeDataBus(data);
    delay(1);
    digitalWrite(WRN, HIGH);
    delay(1);

    digitalWrite(CS2N, HIGH);
    delay(1);
}


void writeDataBus(int data)
{
    digitalWrite(D0, bitRead(data, 0));
    digitalWrite(D1, bitRead(data, 1));
    digitalWrite(D2, bitRead(data, 2));
    digitalWrite(D3, bitRead(data, 3));
    digitalWrite(D4, bitRead(data, 4));
    digitalWrite(D5, bitRead(data, 5));
    digitalWrite(D6, bitRead(data, 6));
    digitalWrite(D7, bitRead(data, 7));
}

void setDataBusToHighZ() {
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);
}

void setBusesToOutput() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(RDN, OUTPUT);
    pinMode(WRN, OUTPUT);
    pinMode(CS2N, OUTPUT);
    pinMode(MR, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
}

void setBusesToLow() {
//  digitalWrite(A0, LOW);
//digitalWrite(A1, LOW);
//digitalWrite(A2, LOW);
//digitalWrite(A5, LOW);

//Register set
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
}




void initUart() {
    digitalWrite(13,HIGH);
    setBusesToOutput();
    setBusesToLow();
    delay(50);

//            8 2 5 0   R e g i s t e r s
//offset  name  Function    Use
//--------------------------------------------------------------------------
//  0*  DATA  Data Read/Write Reg.  Line I/O
//  1*  IER Interrupt Enable Reg. Enable Tx, Rx, RxError, Modem int.
//  2 IID Interrupt ID Reg. ID of highest interrupt source.
//  3 LCR Line Control Reg. Line control parameters and Break.
//  4 MCR Modem Control Reg.  DTR, RTS, OUT1, OUT2 and loopback.
//  5 LSR Line Status Reg.  Tx and Rx status (PE, FE, OE)
//  6 MSR Modem Status Reg. CTS, DSR, RI, RLSD & changes.
//
//  0*  DLL Divisor Latch LOW LSB of Baudrate divisor
//  1*  DLH Divisor Latch HIGH  MSB ...


//LCR  - Line Control Register FIRST SETTING TO ACCESS BAUDRATE DIVISOR REGISTER - Address 011
//Controls the output to the Line:
//  0,1 Word length. (00=5, 01=6, 10=7, 11=8)
//  2 Stop bits
//  3,4 Parity control (00, 10=none, 01 = Odd, 11 = Even)
//  5 Stick parity (0 = normal, 1 = obtuse!)
//  6 Send Break.
//  7 DLA, Divisor Latch Access Bit. (1 = BaudRate divisor latch access) //READ DATASHEET

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN,HIGH);

    delay(50);

//Address set
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);

    delay(50);

    digitalWrite(CS2N,LOW);

    delay(50);

    digitalWrite(WRN,LOW);

//Register set
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, HIGH);

    delay(50);

    digitalWrite(WRN,HIGH);

    delay(50);

//DIVISOR LATCH SETTING - SEE DATASHEET TABLE III.
//I'm using a 4.194304 MHz crystal, and according to the datasheet we must use latch divisor 20 for a 3.072 MHz crystal, so we have to program (4.19/3.072)*20=27.306666 ~27 into the divisor latch.
//27 = 0b00011011 (baud 9600)
//874 = 0b1101101010 (baud 300)
//39 = 0b100111 (baud 9600 with 6mhz crystal)
//DLLSB - Divisor Latch LSB - Address 000 when DLAB is active

//1.8432 Mhz = 12 divisor for 9600 baud
//12 = 0b00001100

//  0 - Bit 0
//  1 - Bit 1 ... 7 - Bit 7

    delay(50);

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN,HIGH);

    delay(50);

//Address set
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);

    delay(50);

    digitalWrite(CS2N,LOW);

    delay(50);

    digitalWrite(WRN,LOW);

//Register set
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);

    delay(50);

    digitalWrite(WRN,HIGH);

    delay(50);

//DLMSB - Divisor Latch MSB - Address 001 when DLAB is active
//  0 - Bit 8
//  1 - Bit 9 ... 7 - Bit 15


    delay(50);
    digitalWrite(RDN, HIGH);
    digitalWrite(WRN,HIGH);

    delay(50);

//Address set
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);

    delay(50);

    digitalWrite(CS2N,LOW);

    delay(50);

    digitalWrite(WRN,LOW);

//Register set
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);

    delay(50);

    digitalWrite(WRN,HIGH);

    delay(50);

//MCR - Modem Control Register - Address 100
//  0  DTR (pin 4) out
//  1 RTS (pin 20) out
//  2 OUT1, N/C on PC H/W.
//  3 OUT2, Enable the IRQ4 signal to the PIC(s) on a PC.
//  4 Loopback, test 8250 internally.
// 5-7  0

//
//delay(50);
//digitalWrite(WRN,LOW);
//delay(50);
////Address set
//digitalWrite(A0, LOW);
//digitalWrite(A1, LOW);
//digitalWrite(A2, LOW);
//
////Register set
//digitalWrite(D0, LOW);
//digitalWrite(D1, LOW);
//digitalWrite(D2, LOW);
//digitalWrite(D3, LOW);
//digitalWrite(D4, HIGH);
//digitalWrite(D5, LOW);
//digitalWrite(D6, LOW);
//digitalWrite(D7, LOW);

//LCR  - Line Control Register SECOND SETTING TO DISABLE ACCESS TO BAUDRATE DIVISOR REGISTER IN ORDER TO SET INTERRUPT ENABLE REGISTER
//Controls the output to the Line:
//  0,1 Word length. (00=5, 01=6, 10=7, 11=8)
//  2 Stop bits
//  3,4 Parity control (00, 10=none, 01 = Odd, 11 = Even)
//  5 Stick parity (0 = normal, 1 = obtuse!)
//  6 Send Break.
//  7 DLA, Divisor Latch Access Bit. (1 = BaudRate divisor latch access) //READ DATASHEET


    delay(50);

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN,HIGH);

    delay(50);

//Address set
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);

    delay(50);

    digitalWrite(CS2N,LOW);

    delay(50);

    digitalWrite(WRN,LOW);

//Register set
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW); //We set DLAB 0 so that we can acces IER and DATA registers

    delay(50);

    digitalWrite(WRN,HIGH);

    delay(50);

//IER - Interrupt Enable Register - Address 001 when DLAB is 0
//When bit 7 of the LCR is 0 the IE register is accessed. There are four sources of interrupt on the 8250 that can be enabled by setting a 1 in the following bits of IER.
//  0  Rx data int. enable
//  1 Tx holding reg. empty int.
//  2 Rx status int. enable (ie Parity, Framing, overrun and BREAK enable).
//  3 Modem signal change int. enable.

//
//delay(50);
//digitalWrite(WRN,LOW);
//delay(50);
////Address set
//digitalWrite(A0, HIGH);
//digitalWrite(A1, LOW);
//digitalWrite(A2, LOW);
//
////Register set
//digitalWrite(D0, HIGH);
//digitalWrite(D1, HIGH);
//digitalWrite(D2, HIGH);
//digitalWrite(D3, HIGH);
//digitalWrite(D4, LOW);
//digitalWrite(D5, LOW);
//digitalWrite(D6, LOW);
//digitalWrite(D7, LOW);


//Set back address bits to DATA = 000, since DLAB is 0, we can access transmit/receive buffer.


    delay(50);

    digitalWrite(RDN, HIGH);
    digitalWrite(WRN,HIGH);

    delay(50);

//Address set
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);

    delay(50);

    digitalWrite(CS2N,LOW);

    delay(50);

    digitalWrite(WRN,LOW);

    delay(50);

    digitalWrite(WRN,HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(300);
    digitalWrite(13,HIGH);
    delay(300);
    digitalWrite(13,LOW);
    delay(300);
    digitalWrite(13,HIGH);
    delay(300);
    digitalWrite(13,LOW);
    delay(300);
    digitalWrite(13,HIGH);
    delay(300);
    digitalWrite(13,LOW);
    delay(300);
}
