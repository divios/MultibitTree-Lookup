#include <stdint.h>

/********************************************************************
 * Constant definitions
 ********************************************************************/
#define IP_ADDRESS_LENGTH 32


/********************************************************************
 * Generate a netmask of length prefixLength
 ********************************************************************/
void getNetmask (int prefixLength, int *netmask);

/********************************************************************
 * Example of a very simple hash function using the modulus operator
 * For more info: https://gist.github.com/cpq/8598442
 ********************************************************************/
int hash(uint32_t IPAddress, int sizeHashTable);

/*
 * Toggles bit of an uint32_t
 */

void toggleBit(uint32_t *dir, int pos);

/*
 * Gets the bit on the desired position of an uint32_t
 */

uint8_t getBit(uint32_t dir, int pos);

/*
 * Sets the bit of an uint32_t
 */

void setBit(uint32_t *dir, int pos, uint8_t newBit);

/*
 * Toggles the flag of a uint16_t, this is the 16 bit
 */

void toggleFlag(uint16_t *dir);

/*
 * Returns the flag of a direction, this is the 16 bit
 */

uint8_t getFlag(uint16_t dir);

/*
 * Returns the content of an uint16_t, this is
 * the value resulting of ignoring the 16 bit
 */

uint16_t getContent(uint16_t dir);

/*
 * Sets the content of an uint16_t, this is
 * the bits 1-15 ignoring the 16 bit
 */

void setContent(uint16_t *dir, uint16_t content);


/*
 * Cuts the prefix with the given start and stop. Range of
 * this two are 1-32
 */

uint32_t applyMask(uint32_t prefix, int start, int stop);

//RL Lab 2020 Switching UC3M
