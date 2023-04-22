import hashlib
import os

print('SeCuRe ENcrYPtoR')


def encrypt_file(fpath):
    with open(fpath, 'rb') as f:
        content = f.read()
    encr = b''
    for byte in content:
        encr += hashlib.md5(bytes([byte])).digest()
    with open(fpath + '.encr_easy', 'wb') as f:
        f.write(encr)


for dirpath, dirs, files in os.walk("."):
    for file in files:
        if file.endswith('.txt'):
            encrypt_file(dirpath + '/' + file)
