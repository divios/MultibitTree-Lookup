#include "utils.h"
#include "stdlib.h"
#include <math.h>

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
    uint16_t content = 0;
    for (int i = 0; i < 15; i++) {
        content += getBit(dir, i) * pow(2, i);
    }
    return content;
}

/*
 * Sets the content of an uint16_t, this is
 * the bits 1-15 ignoring the 16 bit
 */

void setContent(uint16_t *dir, uint16_t content) {
    for (int i = 0; i < 15; i++) {
        setBit((uint32_t*) dir, i, getBit(content, i));
    }
}



/* Applies a given mask */

uint32_t applyMask(uint32_t prefix, int start, int stop) {
    uint32_t mask = 0;
    for (int i = 0; i < 32; i++) {
        if (i >= stop && i <= start - 1) {
            mask += pow(2, i);
        }
    }
    return (prefix & mask);
}



//RL Lab 2020 Switching UC3M
