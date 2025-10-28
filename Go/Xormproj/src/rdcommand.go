package main

import (
	"fmt"
	"os"
)

func readcommand() int {
	filename := "command.in"
	input, err := os.Open(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "the file doesn't exist.\n")
		return 1
	}
	defer input.Close()
	var opt int
	fmt.Fscan(input, &opt)
	return opt
}
