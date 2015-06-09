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
#include <string.h>
#include <alloca.h>

#define FRAME_SIZE 256        
#define TOTAL_NUMBER_OF_FRAMES 256  
#define ADDRESS_MASK  0xFFFF  
#define OFFSET_MASK  0xFF 
#define TLB_SIZE 16       
#define PAGE_TABLE_SIZE 256  

int pageTableNumbers[PAGE_TABLE_SIZE]; 
int pageTableFrames[PAGE_TABLE_SIZE];  

int TLBPageNumber[TLB_SIZE];  
int TLBFrameNumber[TLB_SIZE]; 

int physicalMemory[TOTAL_NUMBER_OF_FRAMES][FRAME_SIZE]; 

int pageFaults = 0;   
int TLBHits = 0;      
int firstAvailableFrame = 0;  
int firstAvailablePageTableNumber = 0;  
int numberOfTLBEntries = 0;             

// number of characters to read for each line from input file
#define BUFFER_SIZE         10

// number of bytes to read
#define CHUNK              	99 

FILE    *address_file;
FILE    *backing_store;

// how we store reads from input file
char    address[BUFFER_SIZE];
int     logical_address;

// the buffer containing reads from backing store
signed char     buffer[CHUNK];

// the value of the byte (signed char) in memory
signed char     value;

void getPage(int address);
void readFromStore(int pageNumber);
void insertIntoTLB(int pageNumber, int frameNumber);

void getPage(int logical_address){
    int pageNumber = ((logical_address & ADDRESS_MASK)>>8);
    int offset = (logical_address & OFFSET_MASK);
    int frameNumber = -1; 
    int i;  
    for(i = 0; i < TLB_SIZE; i++){
        if(TLBPageNumber[i] == pageNumber){  
            frameNumber = TLBFrameNumber[i]; 
                TLBHits++;              
        }
    }
    
	if(frameNumber == -1){
        int i;  
        for(i = 0; i < firstAvailablePageTableNumber; i++){
            if(pageTableNumbers[i] == pageNumber){       
                frameNumber = pageTableFrames[i];      
            }
        }
        if(frameNumber == -1){                     
            readFromStore(pageNumber);            
            pageFaults++;                           
            frameNumber = firstAvailableFrame - 1; 
        }
    }
    
    insertIntoTLB(pageNumber, frameNumber);  
    value = physicalMemory[frameNumber][offset];  
   	printf("frame number: %d\n", frameNumber);
	printf("offset: %d\n", offset); 
    printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, (frameNumber << 8) | offset, value);
}

void insertIntoTLB(int pageNumber, int frameNumber){
    int i;  
    for(i = 0; i < numberOfTLBEntries; i++){
        if(TLBPageNumber[i] == pageNumber){
            break;
        }
    }
    
    if(i == numberOfTLBEntries){
        if(numberOfTLBEntries < TLB_SIZE){  
            TLBPageNumber[numberOfTLBEntries] = pageNumber;    
            TLBFrameNumber[numberOfTLBEntries] = frameNumber;
        }
        else{                                           
            for(i = 0; i < TLB_SIZE - 1; i++){
                TLBPageNumber[i] = TLBPageNumber[i + 1];
                TLBFrameNumber[i] = TLBFrameNumber[i + 1];
            }
            TLBPageNumber[numberOfTLBEntries-1] = pageNumber;  
            TLBFrameNumber[numberOfTLBEntries-1] = frameNumber;
        }        
    }
    
    else{
        for(i = i; i < numberOfTLBEntries - 1; i++){      
            TLBPageNumber[i] = TLBPageNumber[i + 1];     
            TLBFrameNumber[i] = TLBFrameNumber[i + 1];
        }
        if(numberOfTLBEntries < TLB_SIZE){              
            TLBPageNumber[numberOfTLBEntries] = pageNumber;
            TLBFrameNumber[numberOfTLBEntries] = frameNumber;
        }
        else{                                          
            TLBPageNumber[numberOfTLBEntries-1] = pageNumber;
            TLBFrameNumber[numberOfTLBEntries-1] = frameNumber;
        }
    }
    if(numberOfTLBEntries < TLB_SIZE){                
        numberOfTLBEntries++;
    }    
}

void readFromStore(int pageNumber){
    if (fseek(backing_store, pageNumber * CHUNK, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking in backing store\n");
    }
    
    if (fread(buffer, sizeof(signed char), CHUNK, backing_store) == 0) {
        fprintf(stderr, "Error reading from backing store\n");        
    }
    
    int i;
    for(i = 0; i < CHUNK; i++){
        physicalMemory[firstAvailableFrame][i] = buffer[i];
    }
    
    pageTableNumbers[firstAvailablePageTableNumber] = pageNumber;
    pageTableFrames[firstAvailablePageTableNumber] = firstAvailableFrame;
    
    firstAvailableFrame++;
    firstAvailablePageTableNumber++;
}

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
        fprintf(stderr, "Error opening %s%s\n", argv[1]);
        return -1;
    }
    
    // open the file containing the logical addresses
    address_file = fopen(argv[2], "r");
    
    if (address_file == NULL) {
        fprintf(stderr, "Error opening %s\n", argv[2]);
        return -1;
    }
    
    int numberOfTranslatedAddresses = 0;
    // read through the input file and output each logical address
    while ( fgets(address, BUFFER_SIZE, address_file) != NULL) {
        logical_address = atoi(address);
        
		// first seek to byte CHUNK in the backing store
		// SEEK_SET in fseek() seeks from the beginning of the file
		if (fseek(backing_store, CHUNK * i, SEEK_SET) != 0){
			fprintf(stderr, "Error seeking in backing store\n");
			return -1;
		}

		// now read CHUNK byte from the backing store to the buffer
		if(fread(buffer, sizeof(signed char), CHUNK, backing_store) == 0){
			fprintf(stderr, "Error reading from backing store\n");
			return -1;
		}

		// arbitrarily retrieve the 50th byte from the buffer
		value = buffer[50];

		printf("%d \n %d\n", logical_address, value);

		// just so we seek to 10 different spots in the file
		i = (i + 1) % 10;

        getPage(logical_address);
        numberOfTranslatedAddresses++;  // increment the number of translated addresses        
	}
    
    // calculate and print out the stats
    printf("Number of translated addresses = %d\n", numberOfTranslatedAddresses);
    double pfRate = pageFaults / (double)numberOfTranslatedAddresses;
    double TLBRate = TLBHits / (double)numberOfTranslatedAddresses;
    
    printf("Page Faults = %d\n", pageFaults);
    printf("Page Fault Rate = %.3f\n",pfRate);
    printf("TLB Hits = %d\n", TLBHits);
    printf("TLB Hit Rate = %.3f\n", TLBRate);
    
    fclose(address_file);
    fclose(backing_store);
    
    return 0;
}

