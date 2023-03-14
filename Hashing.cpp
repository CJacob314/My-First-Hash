#include "hashing.h"

void Hashing::clearState(){
    memset((void*)state, 0x00, STATE_BUF_LEN);
}

void Hashing::hash(char output[STATE_BUF_LEN], const char* input, const int length){
    clearState();

    uint32_t newLength = 0;
    char* paddedInput = padInput(input, length, newLength);

    unsigned char lbyte = 0x57; // Initalized to 0x57 to start, why not.
    for(uint32_t i = 0; i < newLength; i++){
        unsigned char cbyte = paddedInput[i];

        // Overflows in the state-buffer are INTENTIONAL to make reversal SLIGHTLY more difficult
        state[i % STATE_BUF_LEN] += (cbyte ^ (lbyte | 0x1)) + *(&cbyte + 1) | ~lbyte;
		lbyte = state[i % STATE_BUF_LEN] ^ lbyte;
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
