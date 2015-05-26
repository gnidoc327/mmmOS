/**
 * Demonstration C program illustrating how to perform file I/O for vm assignment.
 *
 * Input file contains logical addresses
 * 
 * Backing Store represents the file being read from disk (the backing store.)
 *
 * We need to perform random input from the backing store using fseek() and fread()
 *
 * This program performs nothing of meaning, rather it is intended to illustrate the basics
 * of I/O for the vm assignment. Using this I/O, you will need to make the necessary adjustments
 * that implement the virtual memory manager.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// number of characters to read for each line from input file
#define BUFFER_SIZE         10

// number of bytes to read
#define CHUNK               99

FILE    *address_file;
FILE    *backing_store;

// how we store reads from input file
char    address[BUFFER_SIZE];

int     logical_address;

// the buffer containing reads from backing store
signed char     buffer[CHUNK];

// the value of the byte (signed char) in memory
signed char     value;

int main(int argc, char *argv[])
{
int i = 0;

    // perform basic error checking
    if (argc != 3) {
        fprintf(stderr,"Usage: ./a.out [backing store] [input file]\n");
        return -1;
    }

    // open the file containing the backing store
    backing_store = fopen(argv[1], "rb");
    
    if (backing_store == NULL) { 
        fprintf(stderr, "Error opening %s\n",argv[1]);
        return -1;
    }

    // open the file containing the logical addresses
    address_file = fopen(argv[2], "r");

    if (address_file == NULL) {
        fprintf(stderr, "Error opening %s\n",argv[2]);
        return -1;
    }

    // read through the input file and output each logical address
    while ( fgets(address, BUFFER_SIZE, address_file) != NULL) {
        logical_address = atoi(address);

        // first seek to byte CHUNK in the backing store
        // SEEK_SET in fseek() seeks from the beginning of the file
        if (fseek(backing_store, CHUNK * i, SEEK_SET) != 0) {
            fprintf(stderr, "Error seeking in backing store\n");
            return -1;
        }

        // now read CHUNK bytes from the backing store to the buffer
        if (fread(buffer, sizeof(signed char), CHUNK, backing_store) == 0) {
            fprintf(stderr, "Error reading from backing store\n");
            return -1;
        }
       
        // arbitrarily retrieve the 50th byte from the buffer 
        value = buffer[50];
         
        printf("%d \t %d\n",logical_address, value);

        // just so we seek to 10 different spots in the file
        i = (i + 1) % 10;
    }


    fclose(address_file);
    fclose(backing_store);

    return 0;
}

