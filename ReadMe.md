# My First Hashing Algorith

This was heavily inspired by SHA-1 and MD6, though it uses much different bit-math.

This is **not** a cryptographic hash function, and collisions are currently **way too common**!

## Current Smallest-Length Collision
ASCII strings `!^L` and `!^M` both hash to hex: `0x27883D9C714DD1EA3180A5F5BCB057`