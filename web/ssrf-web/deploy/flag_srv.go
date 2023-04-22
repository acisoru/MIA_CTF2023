package main

import (
	"fmt"
	"net/http"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "CTF{n0_ssrf_h3r3}")
	})

	err := http.ListenAndServe(":5555", nil)
	if err != nil {
		panic(err)
	}
}
