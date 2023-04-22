import hashlib

hashes = {}
for byte in range(0, 0xFF + 1):
    hashes[hashlib.md5(bytes([byte])).digest()] = byte

with open('file.encr_hard.encr_easy', 'rb') as encf:
    enc_data = encf.read()

with open('stage1.encr_hard', 'wb') as decf:
    for i in range(16, len(enc_data)+16, 16):
        ddta = enc_data[i - 16:i]
        decf.write(bytes([hashes[ddta]]))
