#include "hashing.h"

#include <stdio.h>
#include <iostream>
#include <csignal>

using std::cout;

#define START_MSG "Starting hash algorithm tests...\n" \
                  "Note that this is my first version of my first ever hashing algorithm, and is not by any means cryptographically secure. I intend to use it to compare files and things of that nature.\n" \
                  "This algorithm is coded in c++, using an unsigned character buffer as a state. Very basic bitmath is used to manipulate the state, allowing overflows so that reversal is SOMEWHAT more difficult.\n" \
                  "Enter strings to hash, or Ctrl+D to exit:"

char* toHex(const char* s, size_t len);
void interrupt(int signum);

int main(void){
    signal(SIGINT, interrupt);
    puts(START_MSG);

    char hashed[16] = {'\0'};

    for(std::string s; getline(std::cin, s);){
        Hashing::hash(hashed, s.c_str(), s.length());
        char* hex;
        printf("Hash of \"%s\": %s\n", s.c_str(), hex = toHex(hashed, 15));
        delete[] hex;
    }
    
    puts("Exiting...");

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