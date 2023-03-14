#include <string.h>
#include <cstdint>

#define STATE_BUF_LEN 15

class Hashing {
    private:

    inline static unsigned char state[STATE_BUF_LEN];

    static void clearState();

    // Must be freed by the caller
    static char* padInput(const char* input, const uint32_t length, uint32_t& newLength);

    public:

    static void hash(char output[STATE_BUF_LEN], const char* input, const int length);
};
