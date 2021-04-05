
#include "utils.h"
#include "io.h"
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define MAIN_ARRAY_SIZE 16777216 /* 2^24 */
#define SECOND_ARRAY_SIZE 6400000 /* 2^8 * 25000 */


void intHandler(int signal) {
    //freeIO();
}

// declared here to avoid stackOverflow
uint16_t mainArray[MAIN_ARRAY_SIZE];
uint16_t secondArray[SECOND_ARRAY_SIZE];

int main(int argc, char *argv[]) {

    if (argc != 3) {
        puts("Argumentos erroneos, deben ser 3");
        return 0;
    }

    //signal(SIGINT, intHandler);

    char *fib_table = argv[1];
    char *inputPacketFIle = argv[2];
    int res = initializeIO(fib_table, inputPacketFIle);

    if (res != OK) {
        printIOExplanationError(res);
        exit(-1);
    }

    memset(&mainArray, 0, sizeof(mainArray));
    memset(&secondArray, 0, sizeof(secondArray));

    /* -------------------------------------------------------------------------------*/
    /**** Proceso de leer fichero FIB info y registrar las direcciones en la tabla ****/
    /* -------------------------------------------------------------------------------*/

    uint8_t slot = 0;
    while (1) {

        uint32_t prefix, prefixMasked, max_itin;
        int result, prefixLength, outInterface,
                n_itin = 0;

        result = readFIBLine(&prefix, &prefixLength, &outInterface);
        if (result == REACHED_EOF) {
            puts("Reached end of PIB file");
            break;
        } else if (result != OK) {
            printIOExplanationError(result);
            exit(-1);
        }

        if (prefixLength <= 24) {

            //printf("Es menor que 24\n");
            /* cut the prefix to get the part that the prefixLength marks */
            uint32_t prefixMasked = applyMask(prefix, 32, 32 - prefixLength) >> 8;
            max_itin = pow(2, 24 - prefixLength) - 1;

            do {
                mainArray[prefixMasked] = outInterface;
                prefixMasked++;
                n_itin++;
            } while (n_itin <= max_itin);

        } else {

            //printf("Es mayor que 26\n");
            int rawPos = applyMask(prefix, 32, 8) >> 8,
                    slotAux;

            if (!getFlag(mainArray[rawPos])) { /* If it is the first time that happens */

                uint16_t auxOutput = getContent(mainArray[rawPos]); // the previous interface

                if (auxOutput != 0) {   // we can skip this if it is an invalid prefix
                    for (int i = slot * pow(2, 8);
                         i < slot * pow(2, 8) + pow(2, 8); i++) {
                        secondArray[i] = auxOutput;
                    }
                }
                toggleFlag(&mainArray[rawPos]);
                setContent(&mainArray[rawPos], slot);
                slot++;
            }

            slotAux = getContent(mainArray[rawPos]);

            prefixMasked = applyMask(prefix, 8, 32 - prefixLength);
            max_itin = pow(2, 32 - prefixLength) - 1;

            int position = slotAux * pow(2, 8) + prefixMasked;

            do {
                secondArray[position] = outInterface;
                position++;
                n_itin++;
            } while (n_itin <= max_itin);

        }
    }

/* LOOKUP */

    int processedPackets = 0, totalTableAccesses = 0, totalPacketProcessingTime = 0;

    puts("");

    while (1) {
        int result, n_table_access = 1, output;
        double searchingTime;
        struct timespec initialTime, finalTime;
        uint32_t dir, choppedDir;


        result = readInputPacketFileLine(&dir);

        if (result == REACHED_EOF) {
            puts("Reached end of inputPacket file");
            break;
        } else if (result != OK) {
            printIOExplanationError(result);
            exit(-1);
        }

        clock_gettime(CLOCK_MONOTONIC_RAW,
                      &initialTime);

        choppedDir = applyMask(dir, 32, 8) >> 8;

        if (!getFlag(mainArray[choppedDir])) {
            output = getContent(mainArray[choppedDir]);
        } else {
            uint8_t dirMem = getContent(mainArray[choppedDir]);
            choppedDir = applyMask(dir, 8, 0) + dirMem * pow(2, 8);
            output = secondArray[choppedDir];
            n_table_access++;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW,
                      &finalTime);

        printOutputLine(dir, output,
                        &initialTime, &finalTime,
                        &searchingTime, n_table_access);

        processedPackets++;
        totalTableAccesses += n_table_access;
        totalPacketProcessingTime +=searchingTime;
    }

    printSummary(processedPackets, totalTableAccesses
                                   / processedPackets,
                 totalPacketProcessingTime / processedPackets);
}
