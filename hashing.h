#include <string.h>
#include <cstdint>

class Hashing {
    private:

    inline static unsigned char state[15];

    static void clearState();

    // Must be freed by the caller
    static char* padInput(const char* input, const uint32_t length, uint32_t& newLength);

    public:

    static void hash(char output[15], const char* input, const int length);
};
