package main

import (
	"html/template"
	"log"
	"net/http"
)

type Human struct {
	ID   int
	Name string
}

func homeHandler(w http.ResponseWriter, r *http.Request) {
	rows, err := db.Query("SELECT * FROM people")
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()

	var people []Human
	for rows.Next() {
		var id int
		var name string

		err = rows.Scan(&id, &name)
		if err != nil {
			log.Fatal(err)
		}
		human := Human{id, name}
		people = append(people, human)
	}
	err = rows.Err()
	if err != nil {
		log.Fatal(err)
	}

	data := struct{ Humans []Human }{people}
	tmpl, err := template.ParseFiles("templates/home.html")
	if err != nil {
		log.Fatal(err)
	}
	err = tmpl.Execute(w, data)
	if err != nil {
		log.Fatal(err)
	}
}

func addHandler(w http.ResponseWriter, r *http.Request) {
	name := r.FormValue("name")

	_, err := db.Exec("INSERT INTO people(name) VALUES ('" + name + "')")
	if err != nil {
		w.Write([]byte(err.Error()))
		return
	}

	w.Write([]byte("OK"))
}
