package main

import (
	"fmt"
	"log"
	"time"

	"xorm.io/xorm"
	"xorm.io/xorm/names"
)

type student struct {
	Sno    uint   `xorm:"pk autoincr"`
	Sname  string `xorm:"varchar(20) notnull"`
	Sbirth *time.Time
	Ssex   string
}

func solve_xorm() {
	engine, err := xorm.NewEngine("mysql", "miku:123456@tcp(localhost:3306)/student")
	if err != nil {
		log.Fatal(err)
	}

	defer engine.Close()
	// engine.ShowSQL(true)       // 会在命令行中显示SQL语句
	engine.ShowSQL(true)
	engine.SetMaxOpenConns(10) // 最大连接
	engine.SetMapper(names.SameMapper{})

	var stu student

	_, err = engine.ID(1).Get(&stu)
	if err != nil {
		fmt.Printf("%v\n", err)
	} else {
		fmt.Printf("Get ID(1) query succeed.\n")
	}

	err = engine.Where("Sname = ?", "miku").Iterate(new(student), func(i int, bean interface{}) error {
		stu := bean.(*student)
		fmt.Printf("%v\n", stu)
		return nil
	})
}
