/*
 * Contains keyboard input functions.
 */

#include "keyboard.h"

/* whether keys are pressed or not */
char lshift_pressed = 0;
char rshift_pressed = 0;
char caps_pressed = 0;

/* ring buffer for polling keys */
static unsigned char *ringbuf = 0;
static unsigned int ringstart = 0, ringend = 0, ringsize = 1024;

/*
 * Initializes memory for ring buffer.
 * Must be called before any other keyboard function.
 */
void key_init() {
    ringbuf = malloc(ringsize);
}

/*
 * Returns first key code from enum KeyCode and pressed flag.
 */
void key_decode(int *key, char *pressed) {
    unsigned char c;
    unsigned int ringoldstart = ringstart;
    *key = UNKNOWN;
    *pressed = 0;
    if (ringstart == ringend) {
        return;
    }
    c = ringbuf[ringstart++];
    if (ringstart == ringsize) {
        ringstart = 0;
    }
    if (c == 0xe0) {
        if (ringstart == ringend) {
            ringstart = ringoldstart;
            return;
        }
        c = ringbuf[ringstart++];
        if (ringstart == ringsize) {
            ringstart = 0;
        }
    }
    *pressed = (c >= 0x01) && (c <= 0x6D);
    c &= ~0x80;
    if (c == 0x01) *key = ESCAPE;
    if (c == 0x48) *key = ARROW_UP;
    if (c == 0x4b) *key = ARROW_LEFT;
    if (c == 0x4d) *key = ARROW_RIGHT;
    if (c == 0x50) *key = ARROW_DOWN;
    if (c == 0x1c) *key = ENTER;
}

/*
 * Clears key buffer.
 */
void key_buffer_clear() {
    ringstart = 0;
    ringend = 0;
}

/*
 * Is called mainly from delay_short from time.c.
 * Stores key strokes to buffer every short time if using delay.
 */
void key_poll() {
    unsigned char status = inb(0x64);
    if ((status & 1) && ((status & 0x20) == 0)) {
        ringbuf[ringend++] = inb(0x60);
        if (ringend == ringsize) {
            ringend = 0;
        }
    }
}

/*
 * Reads next key stroke like getchar.
 */
int get_char() {
    unsigned char status = inb(0x64);
    if ((status & 1) && ((status & 0x20) == 0)) {
        int c = inb(0x60);
        char shift_pressed = lshift_pressed | rshift_pressed;
        switch (c) {
        case 2:
            if (shift_pressed) {
                return '!';
            }
            return '1';
        case 3:
            if (shift_pressed) {
                return '@';
            }
            return '2';
        case 4:
            if (shift_pressed) {
                return '#';
            }
            return '3';
        case 5:
            if (shift_pressed) {
                return '$';
            }
            return '4';
        case 6:
            if (shift_pressed) {
                return '%';
            }
            return '5';
        case 7:
            if (shift_pressed) {
                return '^';
            }
            return '6';
        case 8:
            if (shift_pressed) {
                return '&';
            }
            return '7';
        case 9:
            if (shift_pressed) {
                return '*';
            }
            return '8';
        case 0xa:
            if (shift_pressed) {
                return '(';
            }
            return '9';
        case 0xb:
            if (shift_pressed) {
                return ')';
            }
            return '0';
        case 0xc:
            if (shift_pressed) {
                return '_';
            }
            return '-';
        case 0xd:
            if (shift_pressed) {
                return '+';
            }
            return '=';
        case 0xe: // Backspace press
            return '\b';
        case 0xf: // Tab press
            return '\t';
        case 0x10:
            if (shift_pressed | caps_pressed) {
                return 'Q';
            }
            return 'q';
        case 0x11:
            if (shift_pressed | caps_pressed) {
                return 'W';
            }
            return 'w';
        case 0x12:
            if (shift_pressed | caps_pressed) {
                return 'E';
            }
            return 'e';
        case 0x13:
            if (shift_pressed | caps_pressed) {
                return 'R';
            }
            return 'r';
        case 0x14:
            if (shift_pressed | caps_pressed) {
                return 'T';
            }
            return 't';
        case 0x15:
            if (shift_pressed | caps_pressed) {
                return 'Y';
            }
            return 'y';
        case 0x16:
            if (shift_pressed | caps_pressed) {
                return 'U';
            }
            return 'u';
        case 0x17:
            if (shift_pressed | caps_pressed) {
                return 'I';
            }
            return 'i';
        case 0x18:
            if (shift_pressed | caps_pressed) {
                return 'O';
            }
            return 'o';
        case 0x19:
            if (shift_pressed | caps_pressed) {
                return 'P';
            }
            return 'p';
        case 0x1a:
            if (shift_pressed) {
                return '{';
            }
            return '[';
        case 0x1b:
            if (shift_pressed) {
                return '}';
            }
            return ']';
        case 0x1c: // Enter press
            return '\n';
        case 0x1d: // Lctrl press
            return -1;
        case 0x1e:
            if (shift_pressed | caps_pressed) {
                return 'A';
            }
            return 'a';
        case 0x1f:
            if (shift_pressed | caps_pressed) {
                return 'S';
            }
            return 's';
        case 0x20:
            if (shift_pressed | caps_pressed) {
                return 'D';
            }
            return 'd';
        case 0x21:
            if (shift_pressed | caps_pressed) {
                return 'F';
            }
            return 'f';
        case 0x22:
            if (shift_pressed | caps_pressed) {
                return 'G';
            }
            return 'g';
        case 0x23:
            if (shift_pressed | caps_pressed) {
                return 'H';
            }
            return 'h';
        case 0x24:
            if (shift_pressed | caps_pressed) {
                return 'J';
            }
            return 'j';
        case 0x25:
            if (shift_pressed | caps_pressed) {
                return 'K';
            }
            return 'k';
        case 0x26:
            if (shift_pressed | caps_pressed) {
                return 'L';
            }
            return 'l';
        case 0x27:
            if (shift_pressed) {
                return ':';
            }
            return ';';
        case 0x28:
            if (shift_pressed) {
                return '\"';
            }
            return '\'';
        case 0x29:
            if (shift_pressed) {
                return '~';
            }
            return '`';
        case 0x2a: // Lshift press
            lshift_pressed = 1;
            return -1;
        case 0x2b:
            if (shift_pressed) {
                return '|';
            }
            return '\\';
        case 0x2c:
            if (shift_pressed | caps_pressed) {
                return 'Z';
            }
            return 'z';
        case 0x2d:
            if (shift_pressed | caps_pressed) {
                return 'X';
            }
            return 'x';
        case 0x2e:
            if (shift_pressed | caps_pressed) {
                return 'C';
            }
            return 'c';
        case 0x2f:
            if (shift_pressed | caps_pressed) {
                return 'V';
            }
            return 'v';
        case 0x30:
            if (shift_pressed | caps_pressed) {
                return 'B';
            }
            return 'b';
        case 0x31:
            if (shift_pressed | caps_pressed) {
                return 'N';
            }
            return 'n';
        case 0x32:
            if (shift_pressed | caps_pressed) {
                return 'M';
            }
            return 'm';
        case 0x33:
            if (shift_pressed) {
                return '<';
            }
            return ',';
        case 0x34:
            if (shift_pressed) {
                return '>';
            }
            return '.';
        case 0x35:
            if (shift_pressed) {
                return '?';
            }
            return '/';
        case 0x36: // Rshift press
            rshift_pressed = 1;
            return -1;
        case 0x37: // Numpad '*' press
            return -1;
        case 0x38: // Lalt press
            return -1;
        case 0x39: // Space press
            return ' ';
        case 0x3a: // Caps press
            caps_pressed = !caps_pressed;
            return -1;
        case 0x48: // Arrow up press
            return -1;
        case 0x4b: // Arrow left press
            return -1;
        case 0x4d: // Arrow right press
            return -1;
        case 0x50: // Arrow down press
            return -1;  
        case 0xaa: // Lshift release
            lshift_pressed = 0;
            return -1;
        case 0xb6: // Rshift release
            rshift_pressed = 0;
            return -1;
        case 0xba: // Caps release
            return -1;
        }
    }
    return -1;
}
