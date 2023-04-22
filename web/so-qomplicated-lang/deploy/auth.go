package main

import (
	"github.com/golang-jwt/jwt"
	"net/http"
	"time"
)

type Credentials struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

func loginHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method == "GET" {
		http.ServeFile(w, r, "templates/login.html")
		return
	}

	err := r.ParseForm()
	if err != nil {
		w.WriteHeader(http.StatusBadRequest)
		return
	}

	username := r.Form.Get("username")
	password := r.Form.Get("password")

	var dbPassword string
	err = db.QueryRow("SELECT password FROM users WHERE username='" + username + "'").Scan(&dbPassword)

	if err != nil {
		w.WriteHeader(http.StatusUnauthorized)
		w.Write([]byte("no such user"))
		return
	}

	if dbPassword != password {
		w.WriteHeader(http.StatusUnauthorized)
		w.Write([]byte("password is not valid"))
		return
	}

	token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
		"username": username,
		"exp":      time.Now().Add(time.Hour * 24).Unix(),
	})

	tokenString, err := token.SignedString(jwtKey)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		return
	}

	http.SetCookie(w, &http.Cookie{
		Name:    "jwt",
		Value:   tokenString,
		Expires: time.Now().Add(time.Hour * 24),
	})

	http.Redirect(w, r, "/", http.StatusSeeOther)
}

func jwtMiddleware(f func(http.ResponseWriter, *http.Request)) func(http.ResponseWriter, *http.Request) {
	return func(w http.ResponseWriter, r *http.Request) {
		cookie, err := r.Cookie("jwt")
		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			w.Write([]byte("no cookie"))
			return
		}

		token, err := jwt.ParseWithClaims(cookie.Value, &jwt.MapClaims{}, func(token *jwt.Token) (interface{}, error) {
			if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
				return nil, jwt.ErrSignatureInvalid
			}
			return jwtKey, nil
		})

		if err != nil {
			w.WriteHeader(http.StatusUnauthorized)
			w.Write([]byte("signature invalid / parse error"))
			return
		}

		if !token.Valid {
			w.WriteHeader(http.StatusUnauthorized)
			w.Write([]byte("invalid token!"))
			return
		}

		claims, _ := token.Claims.(*jwt.MapClaims)

		exp := int64((*claims)["exp"].(float64))

		if exp < time.Now().Unix() {
			w.WriteHeader(http.StatusUnauthorized)
			w.Write([]byte("token expired"))
			return
		}
		f(w, r)
	}
}
