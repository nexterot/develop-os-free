CC          = gcc
LD          = ld
ASM         = as
CFLAGS      = -Wall -fno-builtin -nostdinc -nostdlib -m32
LFLAGS      = -m elf_i386
ASFLAGS     = -32

loop_first  = /dev/loop7
loop_second = /dev/loop8

OBJFILES = loader.o common/printf.o common/screen.o common/cursor.o kernel.o common/sys.o common/time.o common/memory.o common/keyboard.o

.PHONY: all run clean rebuild
all: bin/kernel.bin bin/disk.img
run:
	sudo qemu-system-i386 -hda bin/disk.img -m 16M
clean:
	@echo "Cleaning workspace..."
	@sudo umount mnt/ || true
	@sudo losetup -d $(loop_first) || true
	@sudo losetup -d $(loop_second) || true
	@sudo rm -rf $(OBJFILES) kernel.bin disk.img tempdir/ mnt/ bin/*
	@echo "Done!"
rebuild: clean all
bin/kernel.bin: $(OBJFILES)
	@echo "Creating kernel..."
	@mkdir -p bin
	@$(LD) -T linker.ld -o $@ $^ $(LFLAGS) 1>/dev/null
bin/disk.img:   
	@echo "Creating image..."
	@dd if=/dev/zero of=$@ bs=512 count=131072 status=none
	
	@echo "Creating partition table..."
	@(echo n; echo p; echo 1; echo 2048; echo 131071; echo a; echo w;) | fdisk $@ 1>/dev/null
	
	@echo "Creating ex2 filesystem..."
	@sudo losetup $(loop_second) $@ -o 1048576 
	@sudo mke2fs $(loop_second) -q
	
	@echo "Installing grub..."
	@mkdir -p mnt
	@sudo mount $(loop_second) mnt
	@sudo mkdir -p mnt/boot
	@sudo losetup $(loop_first) $@
	@sudo grub-install --root-directory=mnt --boot-directory=mnt/boot --no-floppy --modules="normal part_msdos ext2 multiboot biosdisk" $(loop_first) 1>/dev/null
	@sudo cp grub/grub.cfg mnt/boot/grub/grub.cfg
	@sync
	
	@echo "Copying kernel to image..."
	@sudo cp bin/kernel.bin mnt/
	
	@echo "Cleaning up..."
	@sudo umount $(loop_second)
	@sudo rm -rf mnt/
	@sudo losetup -d $(loop_first)
	@sudo losetup -d $(loop_second)
	
	@echo "Success! Output image is '$@'"
.s.o:
	@$(ASM) $(ASFLAGS) -o $@ $< 1>/dev/null
.c.o:
	@$(CC) -Iinclude $(CFLAGS) -o $@ -c $<
