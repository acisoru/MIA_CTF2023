package main

import (
	"crypto/rc4"
	"encoding/hex"
	"flag"
	"os"
)

func EncryptRC4(keyHex string, value []byte) ([]byte, error) {
	key, err := hex.DecodeString(keyHex)
	if err != nil {
		return nil, err
	}

	cipher, err := rc4.NewCipher(key)
	if err != nil {
		return nil, err
	}
	encrypted := make([]byte, len(value))
	cipher.XORKeyStream(encrypted, value)

	return encrypted, nil
}

func main() {
	key := flag.String("key", "", "the key to use for encryption in hex format")
	fname := flag.String("fname", "", "file name for encryption")
	flag.Parse()

	if *key == "" {
		panic("Please specify key")
	}

	if *fname == "" {
		panic("Please specify file to encrypt")
	}

	if len(*key) != 2*20 {
		panic("Bad key length")
	}

	fileContents, _ := os.ReadFile(*fname)
	encrypted, _ := EncryptRC4(*key, fileContents)
	err := os.WriteFile(*fname+".encr_hard", encrypted, 777)
	if err != nil {
		panic(err)
	}

	println("Success")
}
