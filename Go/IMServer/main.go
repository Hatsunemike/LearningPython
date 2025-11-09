package main

import (
	"IMServer/icmpdemo"
	"IMServer/tcpdemo"
	"fmt"
	"log"
	"os"
)

func main() {
	command_file, err := os.Open("./command.in")
	if err != nil {
		log.Fatal(err)
	}
	var opt string
	fmt.Fscanf(command_file, "%s", &opt)
	switch opt {
	case "icmpdemo":
		icmpdemo.Solve()
	case "tcpdemo":
		tcpdemo.Solve()
	default:
		fmt.Println("No command or command isn't exist.")
	}
}
