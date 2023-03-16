#include "hashing.h"

void Hashing::clearState(){
    
    if(!*stateDefault){
        char fullHexData[] = "AC13CC29789CECA70263EEE1E05090";

        for(uint8_t j = 0; j < 15; j++) {
            sscanf(fullHexData + j * 2, "%02hhX", stateDefault + j);
        }
    }


    memcpy((void*)state, (const void*)&stateDefault, STATE_BUF_LEN);


    // memset((void*)state, 0x00, STATE_BUF_LEN);
}

void Hashing::hash(char output[STATE_BUF_LEN], const char* input, const int length){
    clearState();

    uint32_t newLength = 0;
    char* paddedInput = padInput(input, length, newLength);

    unsigned char lbyte = 0x57; // Initalized to 0x57 to start, why not.
    for(uint8_t i = 0; i < PASSES; i++){

        for(uint32_t i = 0; i < newLength; i++){
            unsigned char cbyte = paddedInput[i];

            // Overflows in the state-buffer are INTENTIONAL to make reversal SLIGHTLY more difficult
            state[RING_BUF_INDEX(i)] += (cbyte ^ (lbyte)) + *(&cbyte + 1) | ~lbyte;

            // Below lines are INSPIRED by MD6's compression function as listed in the MIT paper: https://people.csail.mit.edu/rivest/pubs/RABCx08.pdf
            state[RING_BUF_INDEX(i)] ^= (state[RING_BUF_INDEX(i - 1)] ^ state[RING_BUF_INDEX(i - 2)]) ^ state[RING_BUF_INDEX(i - 3)];

            state[RING_BUF_INDEX(i)] ^= (state[RING_BUF_INDEX(i)] >> COMPRESSION_SHIFTS[(uint8_t)((cbyte) & 0xF % 5)]);
                                                            // Grab least significantt 4 bits from cbyte, then modulo 5 to index the shift array (no zero shifts!).
                                                            // XOR the current state byte with the shifted state byte to add more pseudo-randomness.
            lbyte = state[RING_BUF_INDEX(i)]; // Pushes back the first found collision by 10,000. But it is still at 3 characters. 4 characters is the next milestone!
        }
        
    }

    delete[] paddedInput;

    memcpy(output, state, STATE_BUF_LEN);
}

char* Hashing::padInput(const char* input, const uint32_t length, uint32_t& newLength){
    uint32_t paddedLength = 512 - (length % 512);

    if(!paddedLength) // No need to pad if already a multiple of 512
        return (char*)input;
    
    char* paddedInput = new char[newLength = (length + paddedLength)];

    memcpy(paddedInput, input, length);


    // Pad with zeroes, finishing with the original length to prevent collisions
    memset(paddedInput + length, 0x00, paddedLength);
    memcpy(paddedInput + length + paddedLength - 4, &length, 4);

    return paddedInput;
}
