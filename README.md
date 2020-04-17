# Disassembler
This is a disassembler created for a university project written in C. It takes in a file containing machine language and will convert it into its assembly language equivelant.
## Characteristics of machine
* Two’s	complement binary representation
* Stored	program concept with fixed word	length	
* Word addressable	
* 4K	words of main  memory (12 bits per address)	
* 16-bit	instructions (4	for	the	opcode	and	12	for	the	operand)
* A 16-bit accumulator (AC)	
* A 16-bit instruction register (IR)	
* A 16-bit memory buffer register (MBR)	
* A 12-bit program	counter	(PC)	
* A 12-bit memory	address	register	(MAR)	
* A dedicated input and output register 
 
## Assembly Instructions
 * halt: 0000
 * Load X: 0001
 * Store X: 0010
 * Subt X: 0011
 * Add X: 0100
 * Input: 0101
 * Output: 0110
 * Skipcond: 0111
 * Jump X: 1000
 * Mul: 1001
 * Divi: 1010
 * clear: 1011

## Compile and run

### Compile

Ensure [GCC](https://gcc.gnu.org/install/) is installed.
On the command line type:

      dissasembler.c 
    
### Run
To run type one of these commands:

``` ./a.out -d ``` to run default memory content. 

``` ./a.out -c ``` to allow manual imput of machine code.

``` ./a.out -f ``` to run a file in same directory as this program named 'TestFile.txt'.
