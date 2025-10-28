package main

import (
	"fmt"
	"os"
	"time"
)

var chs [10]chan int

func Add(p int, a int, b int) {
	c := a + b
	fmt.Printf("%d\n", c)
	chs[p-1] <- c
}

func solve_channnel() {
	for i := 1; i <= 10; i++ {
		chs[i-1] = make(chan int)
		go Add(i, i, i)
	}
	for _, ch := range chs {
		<-ch
	}
}

func solve_select() {
	ch := make(chan string, 1)
	for T := 1; T <= 10; T++ {
		go func() {
			t := <-ch
			time.Sleep(1e9)
			fmt.Printf("Hello %s\n", t)
		}()
		var tmp string
		fmt.Printf("What's your name?\n")
		fmt.Scan(&tmp)
		ch <- tmp
	}
}

func main() {
	fmt.Println("Hello, World!")
	file, err := os.Open("./command.in")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer file.Close()
	var opt string
	_, err = fmt.Fscanf(file, "%s", &opt)
	if err != nil {
		fmt.Println(err)
		return
	}
	switch opt {
	case "channel":
		solve_channnel()
	case "select":
		solve_select()
	}
}
