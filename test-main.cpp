#include "hashing.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unordered_map>

#define insert(map, k, v) map.insert(std::make_pair<std::string, std::string>(k, v))

using std::cout;

#define START_MSG "Starting hash algorithm tests...\n" \
                  "Note that this is my first version of my first ever hashing algorithm, and is not by any means cryptographically secure. I intend to use it to compare files and things of that nature.\n" \
                  "This algorithm is coded in c++, using an unsigned character buffer as a state. Very basic bitmath is used to manipulate the state, allowing overflows so that reversal is SOMEWHAT more difficult.\n" \
                  "Enter strings to hash, or Ctrl+D to exit:"

char* toHex(const char* s, size_t len);
void interrupt(int signum);
void doInteractive();
void __strGenRecHelper(std::unordered_map<std::string, std::string>& map, std::string prefix, uint8_t len);

int main(int argc, char* argv[]){
    signal(SIGINT, interrupt);


    if(argc > 1 && !strcmp(argv[1], "-i")){
        doInteractive();
        return 0;
    }

    puts("Generating all strings of length 1-10 (with printable ASCII characters) and hashing them...");
    std::unordered_map<std::string, std::string> map; // Storing hashes as strings for easy heap allocation
    char hashed[16] = {'\0'};
    // Generate all strings of length 1-4
    for(uint8_t len = 0; len < 10; len++){
        __strGenRecHelper(map, "", len);
    }
    

    srand(time(NULL));

    return 0;
}

char* toHex(const char* s, size_t len){
    char* buf = new char[len * 2];
    for(size_t i = 0; i < len; i++)
        sprintf(buf + i*2, "%02X", (unsigned char)s[i]);
    return buf;
}

void interrupt(int signum){
    // Clear the ^C, print exiting message, and exit.
    puts("\b\b  \rInterrupt signal received. Exiting...");
    exit(0);
}

void doInteractive(){
    puts(START_MSG);

    char hashed[16] = {'\0'};

    for(std::string s; getline(std::cin, s);){
        Hashing::hash(hashed, s.c_str(), s.length());
        char* hex;
        printf("Hash of \"%s\": %s\n", s.c_str(), hex = toHex(hashed, 15));
        delete[] hex;
    }
    
    puts("Exiting...");
}

void __strGenRecHelper(std::unordered_map<std::string, std::string>& map, std::string prefix, uint8_t len){
    if(len == 0){
        char hashed[16] = {'\0'};
        Hashing::hash(hashed, prefix.c_str(), prefix.length());
        std::string hashStr(hashed, 15);
        
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

        __strGenRecHelper(map, newPrefix, len - 1);
    }
}