package main

import "fmt"

const N = 2000000

var c [N + 10]bool

func solveSimple() {
	/* 传统暴力法 */
	var n int
	fmt.Scanln(&n)
	var (
		a float64
		t int
		k int
	)
	for i := 1; i <= n; i++ {
		fmt.Scanln(&a, &t)
		// fmt.Printf("a==%.6f b==%d\n", a, t)
		for j := 1; j <= t; j++ {
			k = int(float64(j) * a)
			// fmt.Printf("k=%d\n", k)
			if k > N {
				break
			}
			c[k] = !c[k]
		}
		// for j := 1; j <= 30; j++ {
		// 	if c[j] {
		// 		fmt.Printf("1 ")
		// 	} else {
		// 		fmt.Printf("0 ")
		// 	}
		// }
		// fmt.Println("")
	}
	for i := 1; i <= N; i++ {
		if c[i] {
			fmt.Printf("%d\n", i)
			break
		}
	}
}

func solveStd() {
	/* 正解 */
	var n int
	fmt.Scanln(&n)
	var (
		a float64
		t int
	)
	ans := 0
	for i := 1; i <= n; i++ {
		fmt.Scanln(&a, &t)
		for j := 1; j <= t; j++ {
			k := int(float64(j) * a)
			ans ^= k
		}
	}
	fmt.Printf("%d\n", ans)
}

func main() {
	solveStd()
}
