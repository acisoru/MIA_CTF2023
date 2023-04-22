package main

import (
	crand "crypto/rand"
	"database/sql"
	"encoding/base64"
	"fmt"
	"log"
	mrand "math/rand"
	"net/http"
	"os"

	_ "github.com/go-sql-driver/mysql"
)

const letterBytes = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

func RandStringBytes(n int) string {
	b := make([]byte, n)
	for i := range b {
		b[i] = letterBytes[mrand.Intn(len(letterBytes))]
	}
	return string(b)
}

var db *sql.DB
var jwtKey []byte

func getFlag() string {
	flagStr := os.Getenv("FLAG")
	if flagStr == "" { // no flag in env
		flagStr = "CTF{fake_flag}"
	}
	return flagStr
}

func initAdminAccount() {
	var creds Credentials

	creds.Username = "admin_" + RandStringBytes(128)
	creds.Password = getFlag()
	_, _ = db.Exec("INSERT INTO users (username, password) VALUES (?, ?)", creds.Username, creds.Password)

}

func init() {
	tkn := os.Getenv("JWT_TOKEN")
	if tkn == "" {
		jwtKey = make([]byte, 64)
		crand.Read(jwtKey)
	} else {
		jwtKey, _ = base64.StdEncoding.DecodeString(tkn)
	}

	var err error
	db, err = sql.Open("mysql", "testuser:testpassword@tcp(sqldbtaskmia)/testdb?timeout=30s")
	if err != nil {
		log.Fatal(err)
	}

	_, err = db.Exec(`
  CREATE TABLE IF NOT EXISTS people (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255)
);
`)
	if err != nil {
		log.Fatal(err)
	}

	_, err = db.Exec(`
CREATE TABLE IF NOT EXISTS users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(255) UNIQUE,
  password VARCHAR(255)
);
`)
	if err != nil {
		log.Fatal(err)
	}

	initAdminAccount()

}
func main() {
	http.HandleFunc("/", jwtMiddleware(homeHandler))
	http.HandleFunc("/add", jwtMiddleware(addHandler))
	http.HandleFunc("/login", loginHandler)

	fs := http.FileServer(http.Dir("styles"))
	http.Handle("/styles/", http.StripPrefix("/styles/", fs))

	fmt.Println("Listening on port 8080...")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}
