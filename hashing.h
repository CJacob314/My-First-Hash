#include <string.h>
#include <cstdio>
#include <cstdint>

#define STATE_BUF_LEN 15
#define PASSES 16
#define RING_BUF_INDEX(i) (signed int)(i) % STATE_BUF_LEN < 0 ? (signed int)(i) % STATE_BUF_LEN + STATE_BUF_LEN : (signed int)(i) % STATE_BUF_LEN

typedef unsigned __int128 uint128_t; // This did not work the way I wanted, so I am storing the default state value in a char buffer instead.

class Hashing {
    private:

    inline static unsigned char state[STATE_BUF_LEN];
    inline static uint8_t COMPRESSION_SHIFTS[5] = {5, 3, 5, 3, 5};

    // Hex: 0xBA238C4E384DC1CAA1A478FF5FCEF7
    inline static uint8_t stateDefault[STATE_BUF_LEN] = {0xBA, 0x23, 0x8C, 0x4E, 0x38, 0x4D, 0xC1, 0xCA, 0xA1, 0xA4, 0x78, 0xFF, 0x5F, 0xCE, 0xF7};

    static void clearState();

    // Must be freed by the caller
    static unsigned char* padInput(const char* input, const uint32_t length, uint32_t& newLength);

    public:

    static void hash(char output[STATE_BUF_LEN], const char* input, const int length);
};
