# uartsh

Bash like Shell program for small embedded devices using uart.

## Purpose ?
 
To get a mini bash like environment in small devices.

Most of the time when working with small embedded devices we may get stdout working but not stdin.
Though we make stdin working, we need a command line interpreter like shell to run custom commands with arguments.
 
## Solution

I do not want to write a program from scratch, instead make use of possible open source s/w to get a shell like utility.
	
I tried to use gcc's newlib porting guide to make printf, fputs, fgets etc., working with UART.

For command line argument parsing I used Yecheng Fu's [argparse library](https://github.com/cofyc/argparse), a great library.

I ported necessary low level system calls for newlib's stdout and stdin in syscalls.c and written uartsh.c for prompting and handling commands along with arguments.

## How to use example
    
Let's begin with ./uartshConfig.h

This is build configuration file with below macros
		
	#define UARTSH_CONFIG_PROMPT_STRING		"$"

    Specify your shell prompt string

      "willvaLab$"
      "shell$"
      "root#"
      "your-name#"

  	#define UARTSH_CONFIG_uart_getc		serial_getc
  	#define UARTSH_CONFIG_uart_putc		serial_putc

	  Must define hardware specific uart get/put character functions in above macro

	  char serial_getc(); // get a character from UART
	  void serial_putc(char c); // write a character to UART

	#define UARTSH_CONFIG_COMMAND_STRING_SIZE	128

	  Above macro configures buffer size for holding whole command string including arguments.

	  For example, below command has 22 characters _excluding \n and \0_

	  uartsh$ mem -r 32 0x12000000 4

	  Similary other commands may take more or less size based on the requirement.
	  We have to allocate enough buffer space to hold the full command string.
	  I allocated 128 bytes, this size is including 2 special characters \n and \0, so you get 126 bytes for holding the command string alone.

	#define UARTSH_CONFIG_ARGC_MAX			31

	  Above macro is for mentioning how many space separated words would occur in any command string.
	  For example, below command has 5 space separated words

	  uartsh$ mem -r 32 0x12000000 4

	  I allocated max of 32 words for any command string, but you can increase or decrease based on your requirement.
	
### Heap related configuration for newlib's use

    #if 1 // if heap memory is reserved using linker script
        extern char __heap_start; /* Defined by the linker */
        extern char __heap_end;   /* Defined by the linker */
        #define UARTSH_CONFIG_HEAP_START		&__heap_start
        #define UARTSH_CONFIG_HEAP_END			&__heap_end
    #else // if heap memory is reserved with direct address
        #define UARTSH_CONFIG_HEAP_START		0x12000000
        #define UARTSH_CONFIG_HEAP_END			0x12400000
    #endif

      Above macros are for mentioning heap start and end address based on hardware you use and its memory layout.
      If heap memory is reserved using linker script, then you can mention the variable names prefixing with '&' like above, else mention addresses directly.

#### Ignore below macros
	
    #define UARTSH_USE_NEWLIB_FGETS     0
    #define UARTSH_CONFIG_STDIN         stdin
    #define UARTSH_CONFIG_STDOUT        stdout
 
#### How to register commands and open uartsh prompt
    
Define a command as function like below

    #include "uartsh.h"

    int uartshCommand_read(int argc, char* argv[])
    {
        static const char* usages[] = 
        {
            "memread -a #address -c #count",
            "memread --address #address --count #count",
            NULL,
        };

        unsigned int address = 0;
        unsigned int dwords = 0;

        // you can get more information on this from argparse repo [https://github.com/cofyc/argparse]
        UartshCommandOption options[] = {
            UARTSH_OPTION_BEGIN(),
            UARTSH_OPTION_GROUP("memory read options"),
            UARTSH_OPTION_INTEGER('a', "address", &address, "memory address to read"),
            UARTSH_OPTION_INTEGER('c', "count", &dwords, "number of bytes to read"),
            UARTSH_OPTION_END(),
        };

        UartshCommandParser parser = { 0, };
        parser.options = options;
        parser.usages = usages;
        parser.description = "Memory read / write operations";
        argc = uartshParseCommand(&parser, argc, argv);

        if( argc >= 0 )
        {
            volatile unsigned int* memory = (unsigned int*) (address & ~0b11);

            for(int i = 0; i < dwords; i++)
            {
                if( 0 == (i & 0b11) )
                    printf("\n0x%08x:", memory);

                printf(" 0x%08x", *memory++);
            }

            printf("\n");
        }

        return 0; // return values are not handled and of no use
    }

#### Register it in uartshCommandsRegistry.c
		
    #include "./uartsh.h"
    /*-----------------------------------------------*/

    UARTSH_INCLUDE_CMD(uartshCommand_read);  // <--- include the function
    UARTSH_INCLUDE_CMD(uartshCommand_write);
    UARTSH_INCLUDE_CMD(uartshCommand_service);
    UARTSH_INCLUDE_CMD(uartshCommand_someFunction);
    /*-----------------------------------------------*/

    // ------------- register user commands
    const UartshCommand gcUartshCommands[] = {

          UARTSH_REGISTER_CMD("memread", uartshCommand_read),   // <--- register it as a command named "memread"
          UARTSH_REGISTER_CMD("memwrite", uartshCommand_write),
          UARTSH_REGISTER_CMD("service", uartshCommand_service),
          UARTSH_REGISTER_CMD("function", uartshCommand_someFunction),

          // last must end with null
          UARTSH_REGISTER_CMD((void*)0, (void*)0),
    };
	
#### Now in main.c call uartshOpen()

    #include "uartsh.h"

    int main()
    {
    	// if using getchar() then make it to raw mode (no bufferring)
    	setvbuf(stdin, NULL, _IONBF, 0);
	
        extern const UartshCommand gcUartshCommands[];
        uartshOpen(gcUartshCommands);

        // to break the uartsh loop and come here, type exit

        return 0;
    }

### Output:

    uartsh$ ls

    Supported commands are:

        memread
        memwrite
        service
        function

    uartsh$ memread -h

    Usage: memread -a #address -c #count
    or: memread --address #address --count #count

    Memory read / write operations

    -h, --help            show this help message and exit

    memory read options
    -a, --address=<int>   memory address to read
    -c, --count=<int>     number of bytes to read

    uartsh$ memread -a 0x10000000 -c 8

    0x10000000: 0x12345678 0xabcdef12 0x97171723 0x98791870
    0x10000010: 0x12345678 0xabcdef12 0x97171723 0x98791870

    uartsh$ memread --address 0x10000000 -c 8

    0x10000000: 0x12345678 0xabcdef12 0x97171723 0x98791870
    0x10000010: 0x12345678 0xabcdef12 0x97171723 0x98791870

    uartsh$


Improvements and suggestions are always welcome, lets learn together
	
And please check [https://github.com/cofyc/argparse] for details on all supported command options

**Author: Chandra Mohan C [willvalab@gmail.com]**
