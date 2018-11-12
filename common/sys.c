/*
 * Contains system constants and port read & write functions
 */

#include "sys.h"

static unsigned long int next = 1;

unsigned char inb(unsigned short int port) {
    unsigned char res;
    __asm__ volatile (
        "inb %1, %0"
        : "=a"(res)
        : "Nd"(port)
    );
    return res;
}

void outb(unsigned char value, unsigned short int port) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}
 
int rand() { 
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;    // RAND_MAX assumed to be 32767
}
 
void srand(unsigned int seed) {
    next = seed;
}

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

void rtc_seed() {
    unsigned char second = get_RTC_register(0x00);
    unsigned char hour = get_RTC_register(0x04);
    unsigned char day = get_RTC_register(0x07);
    next = second * hour * day;
}
