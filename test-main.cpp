#include "hashing.h"
#include "include/SHA256/SHA256.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unordered_map>
#include <random>
#include <limits.h>
// #include <sys/random.h> // Tried and failed to get Linux getrandom working on my WSL machine.
                        // It will be easier to cross-platform in the future anyways without this

#ifdef _MSC_VER
    # include <intrin.h>
#else
    # include <x86intrin.h>
#endif

#define MARKUSED(X)  ((void)(&(X)))

#define insert(map, k, v) map.insert(std::make_pair<std::string, std::string>(k, v))

using std::cout;

#define START_MSG "Starting hash algorithm tests...\n" \
                  "Note that this is my first ever hashing algorithm, and is not by any means cryptographically secure. I intend to use it to compare files and things of that nature.\n" \
                  "This algorithm is coded in c++, using an byte buffer state. Very basic bitmath is used to manipulate the state, allowing overflows so that reversal is SOMEWHAT more difficult.\n" \
                  "Enter strings to hash, or Ctrl+D to exit:"

char* toHex(const char* s, size_t len);
void interrupt(int signum);
void doInteractive(int withSHA256 = 0);
void __strGenRecHelper(std::unordered_map<std::string, std::string>& map, std::string prefix, uint8_t len, short withSHA256 = 0);
void doBirthdayAttack();
inline uint64_t readTSCp();
inline uint64_t readTSC();

int main(int argc, char* argv[]){
    signal(SIGINT, interrupt);
    srand(time(NULL));

    if(argc > 1){
        if(!strcmp(argv[1], "--time-test")){
            std::random_device rd;
            std::uniform_int_distribution<unsigned long long> gen(0, ULLONG_MAX);
            uint64_t sum50 = 0;
            for(uint8_t i = 0; i < 50; i++){
                int j = 0;
                uint64_t start = readTSCp();
                while(j < 1000){
                    gen(rd);
                    j++;
                }
                uint64_t end = readTSCp();
                sum50 += (end - start);
            }
            printf("Average time (50 runs) to generate 1000 numbers with std::uniform_int_distribution: %llu\n", sum50 / 50ULL);
            sum50 = 0;
            for(uint8_t i = 0; i < 50; i++){
                int j = 0;
                uint64_t start = readTSCp();
                while(j < 1000){
                    uint32_t r = rand() % 5000;
                    MARKUSED(r);
                    j++;
                }
                uint64_t end = readTSCp();
                sum50 += (end - start);
            }
            
            printf("Average time (50 runs) to generate 1000 numbers with rand(): %llu\n", sum50 / 50ULL);
            
            exit(0);

        } else if(!strcmp(argv[1], "-i")){
            if(argc > 2 && (!strcmp(argv[2], "sha256") || !strcmp(argv[2], "SHA256")))
                doInteractive(1);
            else
                doInteractive(0);

            return 0;
        }

        // SHA256 code from: https://github.com/System-Glitch/SHA256.git
        // Thank you to GitHub users System-Glitch, Lambourl, LeStahL (Alexander Kraus)
        if(!strcmp(argv[1], "sha256") || !strcmp(argv[1], "SHA256")){
            puts("Generating all strings of length 1-10 (with printable ASCII characters) and hashing them WITH SHA256...");
            std::unordered_map<std::string, std::string> map; // Storing hashes as strings for easy heap allocation

            // Generate all strings of length 1-4
            for(uint8_t len = 0; len < 10; len++){
                __strGenRecHelper(map, "", len, 1);
            }

            return 0;
        }

        if(!strcmp(argv[1], "--birthday")){
            doBirthdayAttack();
            return 0;
        }
    }

    puts("Generating all strings of length 1-10 (with printable ASCII characters) and hashing them...");
    std::unordered_map<std::string, std::string> map; // Storing hashes as strings for easy heap allocation
    
    // Generate all strings of length 1-4
    for(uint8_t len = 0; len < 10; len++){
        __strGenRecHelper(map, "", len);
    }
    
    return 0;
}

char* toHex(const char* s, size_t len){
    char* buf = new char[len * 2];
    for(size_t i = 0; i < len; i++)
        sprintf(buf + i*2, "%02X", (unsigned char)s[i]);
    return buf;
}

char* toHex(const char* s, size_t len, char output[]){
    for(size_t i = 0; i < len; i++)
        sprintf(output + i*2, "%02X", (unsigned char)s[i]);
    return output;
}

void interrupt(int signum){
    // Clear the ^C, print exiting message, and exit.
    puts("\b\b  \rInterrupt signal received. Exiting...");
    exit(0);
}

void doInteractive(int withSHA256){
    puts(START_MSG);

    char hashed[16] = {'\0'};

    for(std::string s; getline(std::cin, s);){

        if(!withSHA256){
            Hashing::hash(hashed, s.c_str(), s.length());
            char* hex;
            printf("Hash of \"%s\": %s\n", s.c_str(), hex = toHex(hashed, 15));
            delete[] hex;
        } else {
            SHA256 sha256;
            sha256.update(s);
            uint8_t* digest = sha256.digest();
            cout << "Hash of \"" << s << "\": " << SHA256::toString(digest) << "\n";
            delete[] digest;
        }
    }
    
    puts("Exiting...");
}

void __strGenRecHelper(std::unordered_map<std::string, std::string>& map, std::string prefix, uint8_t len, short withSHA256){
    if(map.size() && map.size() % 100000 == 0){
        printf("[MILESTONE] Generated %lu strings so far! Currently on string \"%s\"\n", map.size(), prefix.c_str());
    }
    
    if(len == 0){
        char hashed[16] = {'\0'};
        
        std::string hashStr;
        if(!withSHA256){
            Hashing::hash(hashed, prefix.c_str(), prefix.length());
            hashStr = std::string(hashed, 15);
        } else {
            SHA256 sha256;
            sha256.update(prefix);
            uint8_t* digest = sha256.digest();
            hashStr = std::string(SHA256::toString(digest));
            delete[] digest;
        }
        
        // Check if hash already exists in map, and if so, print out the collision generating input strings.
        if(map.find(hashStr) != map.end()){
            char* hashHex = toHex(hashed, 15);
            printf("Collision found after %lu elements! Input strings \"%s\" and \"%s\" both hash to (hex bytes) 0x%s\n", map.size(), map[hashStr].c_str(), prefix.c_str(), hashHex);
            delete[] hashHex;
        }

        insert(map, static_cast<std::string>(hashStr), std::string(prefix));
        return;
    }

    // ASCII printable characters range from 33-126 inclusive.
    for (uint16_t i = 33; i <= 126; i++){
        std::string newPrefix = prefix + (char)(i);

        __strGenRecHelper(map, newPrefix, len - 1, withSHA256);
    }
}

void doBirthdayAttack(){
    cout << "Starting birthday attack on my hash function. Will update every 10,000 tries.\n";
    std::random_device rd;
    std::uniform_int_distribution<uint16_t> lenGen(10, 1000); // Reduced from 10000 to increase speed

    uint32_t tries = 0;
    std::unordered_map<std::string, std::string> map;

    char hash[STATE_BUF_LEN];

    while(1){
        uint16_t len = lenGen(rd);
        char* data = new char[len];

        // Single byte repeated rand() calling is MUCH faster than std::uniform_int_distribution generating entire chunks at a time, even with 8-byte chunks.
        for(uint32_t i = 0; i < len; i++){
            data[i] = rand() % 256;
        }

        Hashing::hash(hash, data, len);

        std::string hashStr = std::string(hash, STATE_BUF_LEN);
        if(map.find(hashStr) != map.end()){
            char* hex;
            cout << "[COLLISION] Collision found after " << tries << " tries! Input strings \"" << map[hashStr] << "\" and \"" << (hex = toHex(data, len)) << "\" both hash to (hex bytes) 0x" << toHex(hash, STATE_BUF_LEN) << "\n";
            delete[] hex;
            return;
        }


        char dataHex[len * 2 + 1] = {'\0'};
        toHex(data, len, dataHex);
        insert(map, static_cast<std::string>(hashStr), std::string(dataHex));
        // delete[] dataHex;

        tries++;

        if(tries % 10000 == 0){
            char hex[len * 2 + 1] = {'\0'};
            printf("[BIRTHDAY MILESTONE] %u tries so far! Currently on string 0x%s\n", tries, (toHex(data, len, hex)));
        }

        delete[] data;
    }
}


// Below two functions taken from: https://stackoverflow.com/questions/9887839/how-to-count-clock-cycles-with-rdtsc-in-gcc-x86

inline uint64_t readTSC(){
    // _mm_lfence();  // optionally wait for earlier insns to retire before reading the clock
    uint64_t tsc = __rdtsc();
    // _mm_lfence();  // optionally block later instructions until rdtsc retires
    return tsc;
}

// requires a Nehalem or newer CPU.  Not Core2 or earlier.  IDK when AMD added it.
inline uint64_t readTSCp(){
    unsigned dummy;
    return __rdtscp(&dummy);  // waits for earlier insns to retire, but allows later to start
}