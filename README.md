# Start developing OS-free!
Have you ever dreamt of coding an absolutely platform-independent program?
## Make C code that runs without OS!
I am working on a framework that is to simplify C language development at low level.
### History
This is my 5th term course work on the "Operating Systems" subject at the [Department of Computer Science and Technologies](https://github.com/bmstu-iu9), [Bauman Moscow State Technical University](http://www.bmstu.ru/).
### Brief description
The main aim of the coursework is to implement native C libs' functions for I/O tasks (e.g., putchat, printf, gets, scanf) and memory layer (i.e., these are functions like malloc, free, and so on) so that every programmer could write C source code only (using standart libs) and finally get a complete bootable image executing the code. 
### What is done by far
- A minimal OS kernel supporting [Multiboot](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html) specification;
- Embedding [grub2](https://www.gnu.org/software/grub/) bootloader;
- Input functions: getchar, gets;
- Output functions: putchar, puts; also printf function taken from other source;
- Cursor functions: disable_cursor, enable_cursor, move_cursor, update_cursor;
- Time functions: delay, sleeps;
- Memory functions: malloc, free;
- Random functions: rand, srand, rtc_seed;
- Example application: the Tetris game.
### How it actually works
Project build with [Make](https://www.gnu.org/software/make/) tool. [Makefile](https://github.com/nexterot/develop-os-free/blob/master/Makefile) contains all the compiling and building logic. Here are main steps:
1. Kernel compilation (32 bit);
2. Disk image creation and partitioning (using ext2 filesystem);
3. Installing grub into image via grub-install tool;
4. Embedding kernel into image.
### Dependencies
- make
- gcc
- as
- ld
- grub2
- qemu
### Supported hosts
**Linux**. Currently developing and testing on Ubuntu 18.04 LTS minimal.
### How to install
First make sure you have all tools listed in **Dependencies** block installed. Then do the following:
```bash
spam@eggs:~$ git clone https://github.com/nexterot/develop-os-free
spam@eggs:~$ cd develop-os-free
spam@eggs:~$ make
```
In case of errors, or just to clean the directory:
```bash
spam@eggs:~$ make clean
```
Also, the following command is an alias for ```make clean && make```:
```bash
spam@eggs:~$ make rebuild
```
### How to test
To test it with [qemu](https://www.qemu.org/) emulator, run:
```bash
spam@eggs:~$ make run
```

### If you ran into some problem
<details><summary>Details</summary>
  
<p>
  
#### Problem:
```
losetup: bin/disk.img: failed to set up loop device: Device or resource busy
```
#### Solution:<br>
Look up busy loop devices:
```bash
spam@eggs:~$ losetup -a
```
Then edit these lines in Makefile, changing '/dev/loop2' or '/dev/loop3' to a free loop device:
```
loop_first  = /dev/loop2
loop_second = /dev/loop3
```
Finally do:
```bash
spam@eggs:~$ make rebuild
```

</p>
</details>
