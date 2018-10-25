/*
 * Contains functions to work with time.
 */

#include "time.h"

static void delay_short(unsigned int x) {
	const unsigned short TIME = 20000;
	unsigned short t;
	unsigned char low, high;
	
	t = x + TIME;
	
	outb(0b00110000, 0x43); /* set PIT mode to access lo+hi */
	outb(t & 0xFF, 0x40);   /* set lo */
	outb(t >> 8, 0x40);     /* set hi */

	do {
		outb(0b11100010, 0x43);         /* Send read back command */
	} while (inb(0x40) & 0b01000000);   /* receive read back status byte & check not null */
	
	while (t > TIME) {
		/* Get counter */
		outb(0, 0x43);
		low = inb(0x40);
		high = inb(0x40);
		t = ((unsigned short)low) | (((unsigned short) high) << 8);
	}
}

/* 
 * Waits for 'x' ticks.
 */
void delay(unsigned int x) {
	const unsigned int TIME = 30000;
	for (; x > TIME; x -= TIME) {
		delay_short(TIME);
	}
	delay_short(TIME);
}

/*
 * Sleeps for 'seconds' seconds.
 */
void sleeps(unsigned int seconds) {
	for (unsigned int i = 0; i < seconds; i++) {
		delay(SECOND);
	}
}

