package main

import (
	"fmt"
	"io"
	"os"
	"time"
)

const N int = 100000

type node struct {
	l   int
	r   int
	sum int64
	tag int64
}

var tree [N*4 + 10]node
var a [N + 10]int

func pushup(t int) {
	lson := t << 1
	rson := lson | 1
	tree[t].sum = tree[lson].sum + tree[rson].sum
}

func build(t int, l int, r int) {
	tree[t].l = l
	tree[t].r = r
	if l == r {
		tree[t].sum = int64(a[l])
		return
	}
	lson := t << 1
	rson := lson | 1
	mid := (l + r) >> 1
	tree[t].tag = 0
	build(lson, l, mid)
	build(rson, mid+1, r)
	pushup(t)
}

func add(t int, plus int64) {
	tree[t].tag += plus
	tree[t].sum += int64(tree[t].r-tree[t].l+1) * plus
}

func pushdown(t int) {
	if tree[t].tag == 0 {
		return
	}
	if tree[t].l == tree[t].r {
		tree[t].tag = 0
		return
	}
	lson := t << 1
	rson := lson | 1
	add(lson, tree[t].tag)
	add(rson, tree[t].tag)
	tree[t].tag = 0
}

func update(t int, l int, r int, plus int) {
	if r < tree[t].l || l > tree[t].r {
		return
	}
	if l <= tree[t].l && tree[t].r <= r {
		add(t, int64(plus))
		return
	}
	pushdown(t)
	lson := t << 1
	rson := lson | 1
	update(lson, l, r, plus)
	update(rson, l, r, plus)
	pushup(t)
}

func query(t int, l int, r int) int64 {
	if r < tree[t].l || l > tree[t].r {
		return 0
	}
	if l <= tree[t].l && tree[t].r <= r {
		return tree[t].sum
	}
	pushdown(t)
	lson := t << 1
	rson := lson | 1
	return query(lson, l, r) + query(rson, l, r)
}

func solve_fileRead() {
	var file io.Reader
	filename := "1.in"
	file, err := os.Open(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "The input file is fault!")
		return
	}
	// file = os.Stdin
	outFile, err := os.Create("go.out")
	if err != nil {
		fmt.Fprintf(os.Stderr, "The output file is fault!")
		return
	}
	defer outFile.Close()
	var (
		n int
		m int
	)
	fmt.Fscanln(file, &n, &m)
	for i := 1; i <= n; i++ {
		fmt.Fscan(file, &a[i])
	}
	build(1, 1, n)
	var (
		opt int
		l   int
		r   int
		x   int
	)
	for ; m > 0; m-- {
		fmt.Fscan(file, &opt, &l, &r)
		switch opt {
		case 1:
			fmt.Fscan(file, &x)
			update(1, l, r, x)
		case 2:
			fmt.Fprintf(outFile, "%d\n", query(1, l, r))
		}
	}
}

func main() {
	start := time.Now()
	solve_fileRead()
	e := time.Since(start)
	fmt.Printf("%v\n", e)
}
