from pwn import *


flagBytes = bytes.fromhex("cfd8caf7eebce0ffe4bcbfd3e2b8efe4bde2b8bff8fff5edd3ffd3e1b8e0bcebbcf1")
byte = 0x00
print(flagBytes)
for i in range(256):
    flag = xor(flagBytes, byte).hex()
    a = bytes.fromhex(flag)
    if b"CTF" in a:
        print(a)
    byte = byte + 0x01