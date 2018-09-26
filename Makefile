CC      	= gcc
LD      	= ld
ASM         = as
CFLAGS  	= -Wall -fno-builtin -nostdinc -nostdlib -m32
LFLAGS      = -m elf_i386
ASFLAGS     = -32

loop_first  = /dev/loop2
loop_second = /dev/loop3

OBJFILES = loader.o common/printf.o common/screen.o kernel.o

.PHONY: all run clean rebuild
all: kernel.bin disk.img
run: 
	qemu-system-i386 -hda disk.img	
clean:
	@echo "Cleaning workspace..."
	@umount $(loop_second) 2>/dev/null || true
	@losetup -d $(loop_first) 2>/dev/null || true
	@losetup -d $(loop_second) 2>/dev/null || true
	@rm -rf $(OBJFILES) kernel.bin disk.img tempdir/ mnt/
	@echo "Done!"
rebuild: clean all
kernel.bin: $(OBJFILES)
	@echo "Creating kernel..."
	@$(LD) -T linker.ld -o $@ $^ $(LFLAGS) 1>/dev/null
disk.img:	
	@echo "Creating image..."
	@dd if=/dev/zero of=disk.img bs=512 count=131072 status=none
	
	@echo "Creating partition table..."
	@(echo n; echo p; echo 1; echo 2048; echo 131071; echo a; echo w;) | fdisk disk.img 1>/dev/null
	
	@echo "Creating ex2 filesystem..."
	@losetup $(loop_second) disk.img -o 1048576 
	@mke2fs $(loop_second) -q
	
	@echo "Installing grub..."
	@mkdir -p mnt
	@mount $(loop_second) mnt
	@mkdir -p mnt/boot
	@losetup $(loop_first) disk.img
	@grub-install --root-directory=mnt --boot-directory=mnt/boot --no-floppy --modules="normal part_msdos ext2 multiboot biosdisk" $(loop_first) 1>/dev/null
	@cp grub/grub.cfg mnt/boot/grub/grub.cfg
	@sync
	
	@echo "Copying kernel to image..."
	@cp kernel.bin mnt/
	
	@echo "Cleaning up..."
	@umount $(loop_second)
	@rm -rf mnt
	@losetup -d $(loop_first)
	@losetup -d $(loop_second)
	@rm -f kernel.bin
	
	@echo "Success! Output image is 'disk.img'"
.s.o:
	@$(ASM) $(ASFLAGS) -o $@ $< 1>/dev/null
.c.o:
	@$(CC) -Iinclude $(CFLAGS) -o $@ -c $< 2>/dev/null
