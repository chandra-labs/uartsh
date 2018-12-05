### uartsh
  Shell program for small embedded devices using uart

# Purpose ?
   
   To get a mini bash like environment in small devices.
   
   Most of the time when working with small embedded devices we may get stdout working but not stdin. 
   Though we make stdin working, we need a command line interpreter like shell to run custom commands with arguments.
   
# Solution
  
  I do not want to write a program from scratch, instead make use of possible open source s/w to get a shell like utility.
  I tried to use gcc's newlib porting guide to make printf, fputs, fgets etc., working with UART.
  For command line argument parsing I used argparse library by Yecheng Fu [ https://github.com/cofyc/argparse ]
  Command handling and newlib stdout, stdin porting is done by me
  
# continuing..
  
  


