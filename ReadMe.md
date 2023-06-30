# My First Hashing Algorithm

This was heavily inspired by SHA-1 and MD6, though it uses much different bit-math.

Please note, this is <u>not a *tested* cryptographic hash function</u>, though it does attempt to use repeated integer overflows to complicate reversal.

## Current Smallest-Length Collision
I have **not yet found a collision** this new version! The birthday attack has so far tested 22,800,000 32-byte strings (not just printable characters), and the old ascending-order printable-characters has not found one after 35,900,000 strings tested!

Then again, if it were a random oracle, to have 50% probaility of a collision I would need to get that number to 1,152,921,504,606,846,976 strings tested...
