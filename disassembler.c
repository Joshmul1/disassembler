#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define M_SIZE 4096 // Memory Size
#define I_SIZE 17 // Instruction Size
#define O_SIZE 13 // Operand Size

//first 4 I_SIZE are opcode, last 12 are instruction
/* Reminders:
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
 */

//Global variables(includes the registers):
char charArrOfBin[I_SIZE];

//Registers:
char ac[I_SIZE]; //Accumulator - holds data values for CPU to process
char ir[I_SIZE]; //Instruction register - holds next instruction to be executed
char mbr[I_SIZE]; //Memory Buffer Register - either data just read from memory or data ready to be written to memory
char pc[O_SIZE]; //Program Counter - address of next instruction in the program
char mar[O_SIZE]; //Memory Address Register - holds memory address of data being referenced
char inReg[I_SIZE]; //Input Register - holds data from input device
char outReg[I_SIZE]; //Output Register - holds data for output device


//Function signatures:
void displayMemory(char mem[M_SIZE][I_SIZE]);

int bin_to_dec(char const m[I_SIZE], int binType);

int which_opcode(int code);

void binIntoCharArray(int const binArr[I_SIZE - 1]);

void load_default_content(char m[M_SIZE][I_SIZE]);

void memory_to_assembly(char mem[M_SIZE][I_SIZE]);

void halt();

void load();

void store(char mem[M_SIZE][I_SIZE]);

void add(char mem[M_SIZE][I_SIZE]);

void subt(char mem[M_SIZE][I_SIZE]);

void mul(char mem[M_SIZE][I_SIZE]);

void divi(char mem[M_SIZE][I_SIZE]);

void skipcond();

void input();

void output();

void clear();

void decimalToBinary(int n);

void runInstruction(int opCode, char memory[M_SIZE][I_SIZE]);

int requiresOperand(int opCode);

void runnerCycle(char mem[M_SIZE][I_SIZE]);

int read_file(const char *filename, char mem[M_SIZE][I_SIZE]);

void prompt_user(char mem[M_SIZE][I_SIZE]);

int main(int argc, char *argv[]) {
    char memory[M_SIZE][I_SIZE];
    if (argc == 2) {
        if (strcmp(argv[1], "-d") == 0) {
            load_default_content(memory);
        } else if (strcmp(argv[1], "-c") == 0) {
            prompt_user(memory);
        } else if (strcmp(argv[1], "-f") == 0) {
            read_file("TestFile.txt", memory);
        }
        memory_to_assembly(memory);
    } else if (argc > 2) {
        printf("Sorry you put in too many arguments\n");
        halt();
    } else if (argc < 2) {
        printf("You had no arguments inputted\n");
        halt();
    } else {
        printf("Sorry, your choice of argument was invalid\n");
        halt();
    }
    runnerCycle(memory);
    return 0;
}

/*
 * This function displays the memory by using a for loop to iterate through it. It stops when it reaches a halt
 * instruction i.e. first four chars of a memory location = '0'.
 */
void displayMemory(char mem[M_SIZE][I_SIZE]) {
    int i, k;
    for (i = 0; i < M_SIZE; i++) {
        if (mem[i][0] == 0 && mem[i][1] == 0 && mem[i][2] == 0 && mem[i][3] == 0)
            break;
        printf("The memory contents at location %d are : ", i);
        for (k = 0; k < I_SIZE - 1; k++) {
            printf("%c", mem[i][k]);
        }
        printf("\n");
    }
}

/*
 * Auxiliary function of ACE which converts machine code instructions found in memory into assembly language and prints
 * them on the screen
 */
void memory_to_assembly(char mem[M_SIZE][I_SIZE]) {
    printf("\n");
    int opcode, instruction;
    for (int i = 0; i < M_SIZE; i++) {
        opcode = bin_to_dec(mem[i], 0);
        which_opcode(opcode);
        if (opcode == 0 || opcode == 5 || opcode == 6 || opcode == 7 || opcode == 11);
        else {
            instruction = bin_to_dec(mem[i], 1);
            printf("%d", instruction);
        }
        if (opcode == 0)
            break;
        printf("\n");
    }
}

/*
 * This function will take in (in form of a char array) either a word of memory, a 12 bit binary number or 16 bit binary
 * number, and then output either an opcode or operand as a decimal number, or just the direct 12 or 16 bit binary
 * number into its decimal equivalent.
 */
int bin_to_dec(char const m[I_SIZE], int binType) {
    /*
     * if binType == 0, then it's an opcode outputted from 16 bit char array.
     * if binType == 1, then it's an instruction outputted from 16 char array.
     * if isOpcode == 2, then it's a 12 bit char array to become decimal.
     * if isOpcode == 3, then it's a 16 bit char array to become decimal.
     */
    int total_dec = 0, j;
    if (binType == 0) {
        for (j = 0; j < 4; j++) {
            if (j == 3) {
                if (m[j] == '1')
                    total_dec += 1;
            } else if (j == 2) {
                if (m[j] == '1')
                    total_dec += 2;
            } else if (j == 1) {
                if (m[j] == '1')
                    total_dec += 4;
            } else {
                //j==0
                if (m[j] == '1')
                    total_dec += 8;
            }
        }
    } else if (binType == 1) {
        for (j = 4; j < I_SIZE - 1; j++) {
            if (j == 15) {
                if (m[j] == '1')
                    total_dec += 1;
            } else if (j == 14) {
                if (m[j] == '1')
                    total_dec += 2;
            } else if (j == 13) {
                if (m[j] == '1')
                    total_dec += 4;
            } else if (j == 12) {
                if (m[j] == '1')
                    total_dec += 8;
            } else if (j == 11) {
                if (m[j] == '1')
                    total_dec += 16;
            } else if (j == 10) {
                if (m[j] == '1')
                    total_dec += 32;
            } else if (j == 9) {
                if (m[j] == '1')
                    total_dec += 64;
            } else if (j == 8) {
                if (m[j] == '1')
                    total_dec += 128;
            } else if (j == 7) {
                if (m[j] == '1')
                    total_dec += 256;
            } else if (j == 6) {
                if (m[j] == '1')
                    total_dec += 512;
            } else if (j == 5) {
                if (m[j] == '1')
                    total_dec += 1024;
            } else {
                //j==4, i.e the starting bit
                if (m[j] == '1')
                    total_dec -= 2048;
            }
        }
    } else if (binType == 2) { //For a 12 bit number with 2's complement
        for (j = 0; j < 12; j++) {
            if (j == 11) {
                if (m[j] == '1')
                    total_dec += 1;
            } else if (j == 10) {
                if (m[j] == '1')
                    total_dec += 2;
            } else if (j == 9) {
                if (m[j] == '1')
                    total_dec += 4;
            } else if (j == 8) {
                if (m[j] == '1')
                    total_dec += 8;
            } else if (j == 7) {
                if (m[j] == '1')
                    total_dec += 16;
            } else if (j == 6) {
                if (m[j] == '1')
                    total_dec += 32;
            } else if (j == 5) {
                if (m[j] == '1')
                    total_dec += 64;
            } else if (j == 4) {
                if (m[j] == '1')
                    total_dec += 128;
            } else if (j == 3) {
                if (m[j] == '1')
                    total_dec += 256;
            } else if (j == 2) {
                if (m[j] == '1')
                    total_dec += 512;
            } else if (j == 1) {
                if (m[j] == '1')
                    total_dec += 1024;
            } else {
                //j==4, i.e the starting bit
                if (m[j] == '1')
                    total_dec -= 2048;
            }
        }
    } else { //For a 16 bit number with 2's complement
        for (j = 0; j < I_SIZE - 1; j++) {
            if (j == 15) {
                if (m[j] == '1')
                    total_dec += 1;
            } else if (j == 14) {
                if (m[j] == '1')
                    total_dec += 2;
            } else if (j == 13) {
                if (m[j] == '1')
                    total_dec += 4;
            } else if (j == 12) {
                if (m[j] == '1')
                    total_dec += 8;
            } else if (j == 11) {
                if (m[j] == '1')
                    total_dec += 16;
            } else if (j == 10) {
                if (m[j] == '1')
                    total_dec += 32;
            } else if (j == 9) {
                if (m[j] == '1')
                    total_dec += 64;
            } else if (j == 8) {
                if (m[j] == '1')
                    total_dec += 128;
            } else if (j == 7) {
                if (m[j] == '1')
                    total_dec += 256;
            } else if (j == 6) {
                if (m[j] == '1')
                    total_dec += 512;
            } else if (j == 5) {
                if (m[j] == '1')
                    total_dec += 1024;
            } else if (j == 4) {
                if (m[j] == '1')
                    total_dec += 2048;
            } else if (j == 3) {
                if (m[j] == '1')
                    total_dec += 4096;
            } else if (j == 2) {
                if (m[j] == '1')
                    total_dec += 8192;
            } else if (j == 1) {
                if (m[j] == '1')
                    total_dec += 16384;
            } else {
                //j==4, i.e the starting bit
                if (m[j] == '1')
                    total_dec -= 32768;
            }
        }

    }
    return total_dec;
}


/*
 * This is a helper function for memory_to_assembly.
 * It will take in an integer then print the integer's corresponding opcode equivalent in assembly language.
 */
int which_opcode(int code) {
    switch (code) {
        case 0:
            printf("halt ");
            break;
        case 1:
            printf("load ");
            break;
        case 2:
            printf("store ");
            break;
        case 3:
            printf("subt ");
            break;
        case 4:
            printf("add ");
            break;
        case 5:
            printf("input ");
            break;
        case 6:
            printf("output ");
            break;
        case 7:
            printf("skipcond ");
            break;
        case 8:
            printf("jump ");
            break;
        case 9:
            printf("mul ");
            break;
        case 10:
            printf("div ");
            break;
        case 11:
            printf("clear ");
            break;
        default:
            break;
    }

}

/*
 * This is a helper method for decimalToBinary.
 * This will use the integer array created in decimalToBinary to create the character array equivalent of it.
 */
void binIntoCharArray(int const binArr[I_SIZE - 1]) {
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        if (binArr[i] == 0)
            charArrOfBin[i] = '0';
        else
            charArrOfBin[i] = '1';
    }
    charArrOfBin[i] = '\0';
}

/*
 * This auxiliary function will load in a default program straight to memory.
 */
void load_default_content(char m[M_SIZE][I_SIZE]) {
    strcpy(m[0], "0001000000010100\0"); //load contents at address 20 into AC **This is later used to subt**
    strcpy(m[1], "0010000000010100\0"); //store AC contents into address 20
    strcpy(m[2], "0001000000010101\0"); //load contents at address 21 into AC **Later used to multiply and divide**
    strcpy(m[3], "0010000000010101\0"); //store AC contents into address 21
    strcpy(m[4], "0001000000010110\0"); //load contents at address 22 into AC **Later used to add**
    strcpy(m[5], "0010000000010110\0"); //store AC contents into address 22
    strcpy(m[6], "0001000000010111\0"); //load contents at address 23 into AC
    strcpy(m[7], "0011000000010100\0"); //subt contents at address 20 from what's in AC
    strcpy(m[8], "0110000000000000\0"); //output the AC (Should be 1)
    strcpy(m[9], "0100000000010110\0"); //add contents at address 22 from what's in AC
    strcpy(m[10], "1001000000010101\0"); //multiply what's at address 21(so two) with what's in AC(should be 5)
    strcpy(m[11], "0110000000010110\0"); //output the AC (Should be 10)

    strcpy(m[12], "0101000000000101\0"); //input some content into AC
    strcpy(m[13], "1010000000010101\0"); //should divide inputted content by what's in address 21
    strcpy(m[14], "0111100000000000\0"); //skipcond
    strcpy(m[15], "0110000000000110\0"); //This output should be skipped
    strcpy(m[16], "1000000000010010\0"); //jump 18
    strcpy(m[17], "0110000000000110\0"); //this output should be skipped
    strcpy(m[18], "0110000000000110\0"); //output the AC (Should be whatever was inputted divided by 2)
    strcpy(m[19], "0000000000000000\0"); //Should Halt the program
    strcpy(m[20], "0000000000001111\0"); //Address 20 here should be 15
    strcpy(m[21], "0000000000000010\0"); //Address 21 here should be 2
    strcpy(m[22], "0000000000000100\0"); //Address 22 here should be 4
    strcpy(m[23], "0000000000010000\0"); //Address 23 here should be 16

    strcpy(pc, "000000000000");
    strcpy(mar, "000000000000");
}

/*****************
   ISA Functions
******************/

// Function halt, exits the program when called. Uses stdlib.
void halt() {
    printf("\n");
    exit(0);
}


// Function load, uses a for loop to copy what's in the mbr into the ac.
void load() {
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        ac[i] = mbr[i];
    }
    ac[i] = '\0';
}

// Function store, takes what's ion the AC put's in the MBR then put's what's in the MBR into the memory location that's in the MAR.
void store(char mem[M_SIZE][I_SIZE]) {
    int location = bin_to_dec(mar, 2);
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        mbr[i] = ac[i];
    }
    mbr[i] = '\0';
    for (i = 0; i < I_SIZE - 1; i++) {
        mem[location][i] = mbr[i];
    }
    mem[location][i] = '\0';
}

/*
 * This ISA instruction will take the value in the AC and add it to the value that is in the memory location MAR, then
 * put that value back into the AC
 */
void add(char mem[M_SIZE][I_SIZE]) {
    int index, fromMem, fromAC, toAC;
    index = bin_to_dec(mar, 2);
    fromMem = bin_to_dec(mem[index], 1);
    fromAC = bin_to_dec(ac, 3);
    toAC = fromAC + fromMem;

    if (toAC > 32767 || toAC < -32767) {
        printf("\nOverflow occurred in the AC\nExiting...\n");
        halt();
    }

    decimalToBinary(toAC);
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        ac[i] = charArrOfBin[i];
    }
    ac[i] = '\0';
}

/*
 * This function will take the value that is in the AC and subtract the value at memory location MAR from it, then put
 * the result back into the AC
 */
void subt(char mem[M_SIZE][I_SIZE]) {
    int index, fromMem, fromAC, toAC;

    index = bin_to_dec(mar, 2);
    fromMem = bin_to_dec(mem[index], 1);
    fromAC = bin_to_dec(ac, 3);
    toAC = fromAC - fromMem;

    if (toAC > 32767 || toAC < -32767) {
        printf("\nOverflow occurred in the AC\nExiting...\n");
        halt();
    }

    decimalToBinary(toAC);
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        ac[i] = charArrOfBin[i];
    }
    ac[i] = '\0';

}

/*
 * This function will take the value in the AC and multiply it with the value at the memory location from MAR, then put
 * the result back into the AC
 */
void mul(char mem[M_SIZE][I_SIZE]) {
    int index, fromMem, fromAC, toAC;
    index = bin_to_dec(mar, 2);
    fromMem = bin_to_dec(mem[index], 1);
    fromAC = bin_to_dec(ac, 3);
    toAC = fromAC * fromMem;

    if (toAC > 32767 || toAC < -32767) {
        printf("\nOverflow occurred in the AC\nExiting...\n");
        halt();
    }

    decimalToBinary(toAC);
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        ac[i] = charArrOfBin[i];
    }
    ac[i] = '\0';
}

/*
 * This function will take the value in the AC and divide it by the value at the memory location from the MAR, then put
 * the output back into the AC
 */
void divi(char mem[M_SIZE][I_SIZE]) {
    int index, fromMem, fromAC, toAC;
    index = bin_to_dec(mar, 2);
    fromMem = bin_to_dec(mem[index], 1);
    fromAC = bin_to_dec(ac, 3);
    if (((fromAC / (float) fromMem) - fromAC / fromMem) >= 0.5)
        toAC = fromAC / fromMem + 1;
    else
        toAC = fromAC / fromMem;

    decimalToBinary(toAC);
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        ac[i] = charArrOfBin[i];
    }
    ac[i] = '\0';
}

/*
 * This function will check to see if there is a skip condition, and if so, it will increment the program count (PC).
 * The skip condition is when the value in the AC is negative AND if the two bits nearest the opcode are '00' then it
 * will skip, or if the AC is 0 AND the two bits nearest the opcode are '01' then it will skip, or if the AC is
 * positive and the two bits nearest the opcode are '10' then it will skip too.
 */
void skipcond() {
    int AC = bin_to_dec(ac, 3);
    int inMem;
    if (mar[0] == '0' && mar[1] == '0')
        inMem = 0;
    else if (mar[0] == '0' && mar[1] == '1')
        inMem = 1;
    else
        inMem = 2;
    if (AC < 0 && inMem == 0 || AC == 0 && inMem == 1 || AC > 0 && inMem == 2) {
        int PC_Val = bin_to_dec(pc, 2);
        PC_Val++;
        decimalToBinary(PC_Val);
        int i;
        for (i = 4; i < I_SIZE - 1; i++) {
            pc[i - 4] = charArrOfBin[i];
        }
        pc[i] = '\0';
    }
}

// Copies what's in the MAR in the AC effectively changing the next instruction to be run.
void jump() {
    strcpy(pc, mar);
}

/*
 * This function will prompt the user to input an instruction then put the instruction into the AC.
 */
void input() {
    printf("\nPlease input a %d bit value : ", I_SIZE - 1);

    scanf(" %s", inReg);

    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        ac[i] = inReg[i];
    }
    ac[i] = '\0';
}

/*
 * This function will output the contents in the AC.
 */
void output() {
    int i;
    for (i = 0; i < I_SIZE - 1; i++) {
        outReg[i] = ac[i];
    }
    outReg[i] = '\0';
    printf("\nThe value in the AC is : %s", outReg);


}

/*
 * Sets the AC to all zeros.
 */
void clear() {
    strcpy(ac, "0000000000000000");
}


// Fetch decode execute cycle and helper functions:

/*
 * Function run instruction, gets an opCode in decimal and calls the function of the corresponding instruction.
 */
void runInstruction(int opCode, char memory[M_SIZE][I_SIZE]) {
    switch (opCode) {
        case 0:
            halt();
            break;
        case 1:
            load();
            break;
        case 2:
            store(memory);
            break;
        case 3:
            subt(memory);
            break;
        case 4:
            add(memory);
            break;
        case 5:
            input();
            break;
        case 6:
            output();
            break;
        case 7:
            skipcond();
            break;
        case 8:
            jump();
            break;
        case 9:
            mul(memory);
            break;
        case 10:
            divi(memory);
            break;
        case 11:
            clear();
            break;
        default:
            break;
    }
}

/*
 * Function requiresOperand, takes an opCode in decimal and returns 1 or 0 depending on if the instruction uses an operand or not.
 */
int requiresOperand(int opCode) {
    switch (opCode) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 1;
        case 3:
            return 1;
        case 4:
            return 1;
        case 5:
            return 0;
        case 6:
            return 0;
        case 7:
            return 1;
        case 8:
            return 1;
        case 9:
            return 1;
        case 10:
            return 1;
        case 11:
            return 0;
        default:
            return -1;

    }

}


// Fetch-Decode-Execute Cycle
/* Copies what's in the PC into the MAR then copies the contents of memory at address MAR into the IR. It then increments the PC by one.
   It decodes the opcode from the IR then puts the operand bits into the MAR.
   If the instruction requires an operand, it copies the contents of memory at adress MAR into the MBR.
   Finally it calls the auxiliary function that calls the right instruction depending on the MBR;
*/
void runnerCycle(char mem[M_SIZE][I_SIZE]) {
    int opCode, inc = 0;

    do {
        int i;
        if (inc == 20) {
            printf("Had to halt");
            halt();
        }

        // copy pc to mar
        for (i = 0; i < 12; i++) {
            mar[i] = pc[i];
        }
        mar[i] = '\0';

        // copy memory at address mar to IR and pc++
        int marLocation = bin_to_dec(mar, 2); // 12 bit number
        for (i = 0; i < I_SIZE - 1; i++) {
            ir[i] = mem[marLocation][i];
        }
        ir[i] = '\0';

        int pcBefore = bin_to_dec(pc, 2); // 12 bit
        decimalToBinary(++pcBefore);

        // Decode instruction
        opCode = bin_to_dec(ir, 0);
        for (i = 4; i < I_SIZE - 1; i++) {
            pc[i - 4] = charArrOfBin[i];
        }
        pc[i - 4] = '\0';

        int answer = requiresOperand(opCode);
        // fetch operand and put in mar
        if (answer) {
            for (i = 4; i < I_SIZE - 1; i++) {
                mar[i - 4] = ir[i];
            }
            mar[i - 4] = '\0';
            // copy instruction at mem[mar] to mbr
            marLocation = bin_to_dec(mar, 2);
            for (i = 0; i < I_SIZE - 1; i++) {
                mbr[i] = mem[marLocation][i];
            }
            mbr[i] = '\0';
        }
        if (opCode == 0) {
            printf("\n\n Data at end of program:\n Accumulator value: %s\n In memory:\n", ac);
            displayMemory(mem);
        }
        runInstruction(opCode, mem); // Function to do that calls instruction function depending on op code

    } while (1);

}

// read_file opens a file passed in as it's first parameter, it opens the file through fopen, if the file exists it keeps getting the characters one by one until the EOF
// EOF = End of file
// The function only works if 16 bits are put line by line
int read_file(const char *filename, char mem[M_SIZE][I_SIZE]) {

    FILE *file = fopen(filename, "r");
    int i = 0, k = 0;
    char c;
    if (file) {
        while ((c = getc(file)) != EOF) {
            if (c == '\n') {
                i++;
                k = 0;
            } else {
                mem[i][k] = c;
                k++;
            }
        }
        fclose(file);
    } else {
        printf("NO FILE");
        return 1;
    }
    return 0;
}

/*
 * This function will convert a given decimal number into its equivalent binary representation. This binary number is
 * then passed to a function which will put it into a char array(charArrOfBin) to be used globally.
 */
void decimalToBinary(int n) {
    int converted[16] = {0};
    int i = 15;
    if (n > 0) {
        while (n > 0) {
            converted[i] = n % 2;
            i--;
            n = n / 2;
        }
    } else if (n == 0) {
        i = 0;
        for (i = 0; i < 16; i++) {
            converted[i] = 0;
        }
    } else { //For n<0
        n = n * (-1);
        n -= 1;
        while (n > 0) {
            converted[i] = n % 2;
            i--;
            n = n / 2;
        }
        for (i = 0; i < 16; i++) {
            if (converted[i] == 1)
                converted[i] = 0;
            else
                converted[i] = 1;
        }
    }
    binIntoCharArray(converted);
}

/*
 * This function will prompt the user to input an instruction, and will keep doing so until the user exits by typing in
 * 'exit'. Each inputted instruction will be stored into memory.
 */
void prompt_user(char mem[M_SIZE][I_SIZE]) {
    int quit = 0, i = 0;
    while (quit == 0) {
        printf("\nPlease input an instruction or type 'exit' to stop: ");
        input();
        if (strcmp(inReg, "exit") == 0) {
            quit = 1;
        } else {
            strcpy(mem[i], inReg);
        }
        i++;
    }
}
