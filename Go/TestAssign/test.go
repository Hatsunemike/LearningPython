package main

import "fmt"

func foo(a []int) {
	a[0] = 1
	a[1] = 1
}

func main() {
	a := [5]int{2, 2, 2, 2, 2}
	foo(a[:])
	fmt.Println(a)
}
