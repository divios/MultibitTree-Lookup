#include "utils.h"
#include "stdlib.h"
#include <math.h>

#define contentMask 32767 // (2^16) - 1

/********************************************************************
 * Generate a netmask of length prefixLength
 ********************************************************************/
void getNetmask(int prefixLength, int *netmask){

	*netmask = (0xFFFFFFFF << (IP_ADDRESS_LENGTH - prefixLength));

}

/********************************************************************
 * Example of a very simple hash function using the modulus operator
 * For more info: https://gist.github.com/cpq/8598442
 ********************************************************************/
int hash(uint32_t IPAddress, int sizeHashTable){

	//Map the key (IPAddress) to the appropriate index of the hash table
  int index = IPAddress % sizeHashTable;
  return (index);

}

/*
 * Toggles bit of an uint32_t
 */

void toggleBit(uint32_t *dir, int pos) {
    *dir ^= 1UL << pos;
}

/*
 * Gets the bit on the desired position of an uint32_t
 */

uint8_t getBit(uint32_t dir, int pos) {
    return (dir >> pos) & 1UL;
}

/*
 * Sets the bit of an uint32_t
 */

void setBit(uint32_t *dir, int pos, uint8_t newBit) {
    if ( (getBit(*dir, pos) ^ newBit) == 1UL) {
        toggleBit(dir, pos);
    }
}

/*
 * Toggles the flag of a uint16_t, this is the 16 bit
 */

void toggleFlag(uint16_t *dir) {
    toggleBit((uint32_t*) dir, 15);
}

/*
 * Returns the flag of a direction, this is the 16 bit
 */
uint8_t getFlag(uint16_t dir) {
    return getBit(dir, 15);
}

/*
 * Returns the content of an uint16_t, this is
 * the value resulting of ignoring the 16 bit
 */

uint16_t getContent(uint16_t dir) {
    return (dir & contentMask);
}

/*
 * Sets the content of an uint16_t, this is
 * the bits 1-15 ignoring the 16 bit
 */

void setContent(uint16_t *dir, uint16_t content) {
    uint16_t mask = (*dir ^ content) & contentMask;
    *dir = (*dir ^ mask) ;
}

/* Applies a given mask */

uint32_t applyMask(uint32_t prefix, int start, int stop) {
    uint32_t mask = pow(2, start) - pow(2, stop);
    return (prefix & mask);
}


//RL Lab 2020 Switching UC3M
