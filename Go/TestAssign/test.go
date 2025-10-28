package main

import "fmt"

func main() {
	a := []int{1, 2, 3}
	b := a
	b[0] = 0
	b[1] = 0
	b[2] = 0
	fmt.Printf("a:\n")
	for _, v := range a {
		fmt.Printf("%d ", v)
	}
	fmt.Println()
	fmt.Printf("b:\n")
	for _, v := range b {
		fmt.Printf("%d ", v)
	}
}
