package main

import (
	"fmt"
	"os"
)

func guibing(a []int, l int, r int) {
	if l == r {
		return
	}
	mid := (l + r) >> 1
	guibing(a, l, mid)
	guibing(a, mid+1, r)
	p := l
	q := mid + 1
	b := make([]int, r-l+1)
	cnt := 0
	for p <= mid && q <= r {
		if a[p] < a[q] {
			b[cnt] = a[p]
			cnt++
			p++
		} else {
			b[cnt] = a[q]
			cnt++
			q++
		}
	}
	for p <= mid {
		b[cnt] = a[p]
		cnt++
		p++
	}
	for q <= r {
		b[cnt] = a[q]
		cnt++
		q++
	}
	for i := 0; i < cnt; i++ {
		a[l+i] = b[i]
	}
}

func main() {
	fin := os.Stdin
	fout := os.Stdout
	if len(os.Args) > 1 {
		var err1 error
		var err2 error
		fin, err1 = os.OpenFile("./input.txt", os.O_RDONLY, 0444)
		fout, err2 = os.OpenFile("./mysort.out", os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
		if err1 != nil || err2 != nil {
			fmt.Fprintf(os.Stderr, "err1: %v\n", err1.Error())
			fmt.Fprintf(os.Stderr, "err2: %v\n", err2.Error())
			return
		}
		defer fin.Close()
		defer fout.Close()
	}
	var n int
	fmt.Fscan(fin, &n)
	a := make([]int, n+1)
	for i := 1; i <= n; i++ {
		fmt.Fscan(fin, &a[i])
	}
	guibing(a, 1, n)
	for i := 1; i <= n; i++ {
		fmt.Fprint(fout, a[i], " ")
	}
	fmt.Fprintln(fout)
}
