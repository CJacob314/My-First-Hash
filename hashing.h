#include <string.h>
#include <cstdio>
#include <cstdint>

#define STATE_BUF_LEN 15
#define PASSES 16
#define RING_BUF_INDEX(i) (signed int)i % STATE_BUF_LEN < 0 ? (signed int)i % STATE_BUF_LEN + STATE_BUF_LEN : (signed int)i % STATE_BUF_LEN

typedef unsigned __int128 uint128_t; // This did not work the way I wanted, so I am storing the default state value in a char buffer instead.

class Hashing {
    private:

    inline static unsigned char state[STATE_BUF_LEN];
    inline static uint8_t COMPRESSION_SHIFTS[5] = {6, 3, 5, 1, 2};
    inline static unsigned char stateDefault[STATE_BUF_LEN] = {0x0};

    static void clearState();

    // Must be freed by the caller
    static char* padInput(const char* input, const uint32_t length, uint32_t& newLength);

    public:

    static void hash(char output[STATE_BUF_LEN], const char* input, const int length);
};
