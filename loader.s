	.text
    .global loader                   # making entry point visible to linker

    .set FLAGS,    0x0               # this is the Multiboot 'flag' field
    .set MAGIC,    0x1BADB002        # 'magic number' lets bootloader find the header
    .set CHECKSUM, -(MAGIC + FLAGS)  # checksum required

    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

# reserve initial kernel stack space
    .set STACKSIZE, 0x4000           # that is, 16k.
    .lcomm stack, STACKSIZE          # reserve 16k stack
    .comm  mbd, 4                    
    .comm  magic, 4                 

loader:
    movl  $(stack + STACKSIZE), %esp # set up the stack
    movl  %eax, magic                # Multiboot magic number
    movl  %ebx, mbd                  # Multiboot data structure
    pushl %eax
    pushl %ebx
    call  main                       # call C code
    cli
hang:
    hlt                              # halt machine should kernel return
    jmp   hang
