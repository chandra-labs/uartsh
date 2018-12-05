### uartsh
  Shell program for small embedded devices using uart

# Purpose ?
   
   To get a mini bash like environment in small devices.
   
   Most of the time when working with small embedded devices we may get stdout working but not stdin. 
   Though we make stdin working, we need a command line interpreter like shell to run custom commands with arguments.
   
# Solution
  
  I do not want to write a program from scratch, instead make use of possible open source s/w to get a shell like utility.
  I tried to use gcc's newlib porting guide to make printf, fputs, fgets etc., working with UART.
  For command line argument parsing I used argparse library by "Yecheng Fu" https://github.com/cofyc/argparse.
  I ported all the low level system calls for newlib's stdout and stdin in syscalls.c.
  Developed uartsh.c for prompting and handling commands along with arguments.
  
# What you get ?
  
  # syscalls.c
    All low level calls needed by newlib to get printf, fgets etc working.
    Added uartsh_gets() and uartsh_puts() functions.
    
  # argparse.c | argparse.h
    Taken from https://github.com/cofyc/argparse and modified to fit the needs.
    Get documents on Howto from above link.
    
  # uartsh.c | uartsh.h
    Provides APIs for running a shell using UART.
    
    1. uartshOpen(); // opens shell prompt and processes registered commands in loop
    2. uartshParseCommand(); // parses command arguments
    3. uartsh_puts(); // for stdout
    4. uartsh_gets(); // for stdin
  
  # uartshCommandsRegistry.c
  will continue..
  


