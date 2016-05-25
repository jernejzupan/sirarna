#ifndef LCD_H_
#define LCD_H_

/****************************************************************************
    LCD-AVR-4d.c  - Use an HD44780U based LCD with an Atmel ATmega processor
 
    Copyright (C) 2013 Donald Weiman    (weimandn@alfredstate.edu)
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/****************************************************************************
		File:			lcd.c
		Date:			May 5, 2016
		Target:			ATmega324
		Compiler:		avr-gcc (AVR Studio 5)
		Modyfier:		Jernej Zupan
		 
		 
		OriginalFile:		LCD-AVR-4d.c
		OriginalDate:		September 16, 2013
		OriginalTarget:		ATmega328
		OriginaCompiler:    avr-gcc (AVR Studio 6)
		OriginalAuthor:		Donald Weiman
 
      Summary:    4-bit data interface, busy flag not implemented.
                  Any LCD pin can be connected to any available I/O port.
                  Includes a simple write string routine.
 */
/******************************* Program Notes ******************************
 
            This program uses a 4-bit data interface but does not use the
              busy flag to determine when the LCD controller is ready.  The
              LCD RW line (pin 5) is not connected to the uP and it must be
              connected to GND for the program to function.
 
            All time delays are longer than those specified in most datasheets
              in order to accommodate slower than normal LCD modules.  This
              requirement is well documented but almost always ignored.  The
              information is in a note at the bottom of the right hand
              (Execution Time) column of the instruction set.
 
  ***************************************************************************
 
            The four data lines as well as the two control lines may be
              implemented on any available I/O pin of any port.  These are
              the connections used for this program:
 
                 -----------                   ----------
                | ATmega328 |                 |   LCD    |
                |           |                 |          |
                |        PA7|---------------->|D7        |
                |        PA6|---------------->|D6        |
                |        PA5|---------------->|D5        |
                |        PA4|---------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |        PA3|---------------->|E         |
                |           |         GND --->|RW        |
                |        PA2|---------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/



#ifndef F_CPU
	#define F_CPU 1000000UL
#endif

// LCD interface (should agree with the diagram above)
//   make sure that the LCD RW pin is connected to GND
#define lcd_D7_port     PORTA                   // lcd D7 connection
#define lcd_D7_bit      PORTA7
#define lcd_D7_ddr      DDRA

#define lcd_D6_port     PORTA                   // lcd D6 connection
#define lcd_D6_bit      PORTA6
#define lcd_D6_ddr      DDRA

#define lcd_D5_port     PORTA                   // lcd D5 connection
#define lcd_D5_bit      PORTA5
#define lcd_D5_ddr      DDRA

#define lcd_D4_port     PORTA                   // lcd D4 connection
#define lcd_D4_bit      PORTA4
#define lcd_D4_ddr      DDRA

#define lcd_E_port      PORTA                   // lcd Enable pin
#define lcd_E_bit       PORTA3
#define lcd_E_ddr       DDRA

#define lcd_RS_port     PORTA                   // lcd Register Select pin
#define lcd_RS_bit      PORTA2
#define lcd_RS_ddr      DDRA

//#define lcd_RW_port     PORTD                   // lcd Enable pin
//#define lcd_RW_bit      PORTD5
//#define lcd_RW_ddr      DDRD

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear				0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home				0b00000010          // return cursor to first position on first line
#define lcd_EntryMode			0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff			0b00001000          // turn display off
#define lcd_DisplayOn			0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset		0b00110000          // reset the LCD
#define lcd_FunctionSet4bit		0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor			0b10000000          // set cursor position
#define lcd_SetAdressToCGRAM	0b01000000			// Set CGRAM adress

// Function Prototypes
void lcd_write_4(uint8_t theByte);
void lcd_write_instruction_4d(uint8_t theInstruction);
void lcd_write_character_4d(uint8_t theData);
void lcd_write_string_4d(uint8_t* theString);
void lcd_init_4d(void);

#endif /* LCD_H_ */