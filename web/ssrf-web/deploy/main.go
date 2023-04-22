package main

import (
	"bytes"
	"fmt"
	"io"
	"mime"
	"net/http"
	"os"
	"path/filepath"
	"strings"
)

func sendRequest(w http.ResponseWriter, r *http.Request) {

	url := r.FormValue("url")
	headersOnly := r.FormValue("headers_only") == "on"

	// Filter requests from localhost and 127.0.0.1
	if strings.Contains(url, "localhost") || strings.Contains(url, "127.0.0.1") {
		http.Error(w, "No cheating please", http.StatusForbidden)
		return
	}

	client := &http.Client{}
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		fmt.Fprintf(w, "Error creating request: %v", err)
		return
	}

	if headersOnly {
		req.Header.Set("Content-Type", "text/plain")
	}

	resp, err := client.Do(req)
	if err != nil {
		fmt.Fprintf(w, "Error sending request: %v", err)
		return
	}
	defer resp.Body.Close()

	// Read the response body only if the checkbox is not checked
	var body []byte
	if !headersOnly {
		body, err = io.ReadAll(resp.Body)
		if err != nil {
			fmt.Fprintf(w, "Error reading response: %v", err)
			return
		}
	}

	// Combine the headers and body into a single response string
	var response bytes.Buffer
	response.WriteString(fmt.Sprintf("HTTP/1.1 %s\r\n", resp.Status))
	for name, values := range resp.Header {
		for _, value := range values {
			response.WriteString(fmt.Sprintf("%s: %s\r\n", name, value))
		}
	}
	response.WriteString("\r\n")
	if !headersOnly {
		response.Write(body)
	}

	fmt.Fprintf(w, "%s", response.String())
}

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		file, err := os.Open("index.html")
		// if there is an error opening the file, return 404
		if err != nil {
			http.NotFound(w, r)
			return
		}
		defer file.Close()

		// set the content type header based on the file extension
		contentType := "text/html"
		if ext := filepath.Ext(r.URL.Path); ext != "" {
			contentType = mime.TypeByExtension(ext)
		}
		w.Header().Set("Content-Type", contentType)

		// copy the file contents to the response writer
		if _, err := io.Copy(w, file); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}
	})

	http.HandleFunc("/send_request", sendRequest)
	http.ListenAndServe(":8080", nil)

	fmt.Println("Listening on :8080 ...")
}
