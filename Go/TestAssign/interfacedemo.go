package main

import "fmt"

type Integer int

func (a Integer) Less(b Integer) bool { // 针对赋值传参的方法
	return a < b
}

func (a *Integer) Add(b Integer) { // 针对指针传参的方法
	(*a) += b
}

type Lesser interface {
	Less(b Integer) bool
}

type Adder interface {
	Add(b Integer)
}

type LessAdder interface {
	Less(b Integer) bool
	Add(b Integer)
}

func solve() {
	var a Integer = 10
	var b Integer = 20
	var lesser Lesser = a
	fmt.Printf("lesser.Less(%d): %v\n", b, lesser.Less(b))

	var c Integer = 5
	var adder Adder = &c
	adder.Add(b)
	fmt.Printf("adder.add(%d)\n", c)

	var lessAdder LessAdder = &c // 这里是可以的，因为Less方法可以把用到成员变量c的地方改为成员变量的指针取值(*(&c))，这样就变成了变量&c的方法
	lessAdder.Add(b)
	fmt.Printf("lessAdder.add(%d)\n", c)

	// var lessAdder2 LessAdder = a
	/* 此时会报错，因为这个接口无法实现add方法：
	cannot use a (variable of int type Integer) as LessAdder value in variable declaration: Integer does not implement LessAdder (method Add has pointer receiver)
	无法通过上面类似的&a代替a *Integer来实现，因为lessAdder2接口所派生的不是一个实对象，而是只有两个a当中的实方法的一个虚对象。
	*/
}
