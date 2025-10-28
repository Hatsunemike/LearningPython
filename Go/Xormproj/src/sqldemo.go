package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

func check_exist(db *sql.DB, name string) bool {
	rows, err := db.Query("SELECT * FROM student WHERE Sname= ?", name)
	if err != nil {
		log.Fatal(err)
	}
	if rows.Next() {
		return true
	} else {
		return false
	}
}

func addNewStu(db *sql.DB, name string) {
	stmt, err := db.Prepare("INSERT INTO student(Sname, Ssex) VALUES(?, 'male')")
	if err != nil {
		log.Fatal(err)
	}
	res, err := stmt.Exec(name)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(res.RowsAffected())
}

func delStu(db *sql.DB, name string) {
	stmt, err := db.Prepare("DELETE FROM student WHERE Sname= ?")
	if err != nil {
		log.Fatal(err)
	}
	res, err := stmt.Exec(name)
	if err != nil {
		log.Fatal(err)
	}
	row, err := res.RowsAffected()
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("delete down, affectedRows: %v\n", row)
}

func solve_sql() {
	db, err := sql.Open("mysql", "miku:123456@tcp(localhost:3306)/student")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	err = db.Ping()
	if err != nil {
		log.Fatal(err)
	}
	rows, err := db.Query("SELECT Sno, Sname FROM student WHERE Ssex='female'")
	if err != nil {
		log.Fatal(err)
	}
	defer rows.Close()
	fmt.Printf("Query results:\n")
	var (
		t_Sno   int
		t_Sname string
	)
	for rows.Next() {
		err := rows.Scan(&t_Sno, &t_Sname)
		if err != nil {
			log.Fatal(err)
		}
		fmt.Printf("Sno: %d Sname: %s\n", t_Sno, t_Sname)
	}

	switch readcommand() {
	case 1:
		addNewStu(db, "Tom")
	case 2:
		delStu(db, "Tom")
	}
}
