
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
