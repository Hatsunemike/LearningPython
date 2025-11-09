package timedemo

import (
	"fmt"
	"time"
)

type node struct {
	t    *time.Time
	name string
}

func (a *node) SetName(name string) {
	a.name = name
}

func Solve() {
	var a node
	a.name = "111"
	fmt.Printf(": %v", a)
}
