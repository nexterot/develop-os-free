#include "time.h"
#include "printf.h"

static void delay_short(unsigned int x) {
	const unsigned short TIME = 20000;
	unsigned short t;
	unsigned char low, high;
	
	t = x + TIME;
	
	// Set PIT mode to access lo+hi
	outb(0b00110000, 0x43);
	// Set lo
	outb(t & 0xFF, 0x40);
	// Set hi
	outb(t >> 8, 0x40);
	// Send read back command and
	// receive read back status byte & check not null
	do {
		outb(0b11100010, 0x43);
	} while (inb(0x40) & 0b01000000);
	
	while (t > TIME) {
		outb(0, 0x43);
		low = inb(0x40);
		high = inb(0x40);
		t = ((unsigned short)low) | (((unsigned short) high) << 8);
	}
}

void delay(unsigned int x) {
	const unsigned int TIME = 30000;
	for (; x > TIME; x -= TIME) {
		delay_short(TIME);
	}
	delay_short(TIME);
}
