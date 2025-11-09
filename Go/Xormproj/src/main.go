package main

import (
	"fmt"
	"os"
)

func studentCheck() {
	var stu student
	fmt.Printf("%v", stu)
}

func main() {
	fmt.Fprintf(os.Stdout, "Hello World!\n")
	solve_xorm()
}
